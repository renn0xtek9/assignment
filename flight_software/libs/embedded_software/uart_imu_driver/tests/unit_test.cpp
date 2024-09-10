// Copyright 2024 <Maxime Haselbauer>
/*! \file unit_test.cpp
 *\brief unit tests of the UartImuDriver.
 */
#include <communication_protocols/uart_imu.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>
#include <serializer/serializer.h>
#include <uart_imu_driver/driver.h>
#include <uart_imu_driver/driver_context.h>

#include <chrono>
#include <mutex>
#include <queue>

#include "fixtures.h"

using namespace std::chrono_literals;  // NOLINT

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

constexpr int FILE_DESCRIPTOR{42}; /**< File descriptor for the device file.*/

/*! \test UartImuDriver: shall not throw when initialized.*/
TEST_F(UartImuDriverTest, UartImuDriverInitialization) {
  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
}

/*! \test UartImuDriver: shall not start if cannot open file.*/
TEST_F(UartImuDriverTest, UartImuDriverWontStartIfCannotOpenFile) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(-1));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  EXPECT_EQ(messages::ImuDriverStatus::NO_DATA, imu_driver_context_.GetStatus())
      << "Status should not turn OK if cannot open file";
  driver.Stop();
}

/*! \test UartImuDriver: can start and stop driver. */
TEST_F(UartImuDriverTest, UartImuDriverStartStop) {
  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(FILE_DESCRIPTOR));
  EXPECT_EQ(messages::ImuDriverStatus::NO_DATA, imu_driver_context_.GetStatus()) << "Initial status should be NO_DATA";
  EXPECT_CALL(os_abstraction_layer_, TimeStampNow()).Times(AtLeast(0));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(FILE_DESCRIPTOR)).WillRepeatedly(Return(0));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(FILE_DESCRIPTOR, _, 1)).Times(AtLeast(0));
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  EXPECT_EQ(messages::ImuDriverStatus::NO_DATA, imu_driver_context_.GetStatus()) << "Status should be OK after start";
  driver.Stop();
}

/*! \test UartImuDriver : flush the device file at start.*/
TEST_F(UartImuDriverTest, UartImuFlushDeviceFileAtStart) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(FILE_DESCRIPTOR));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(FILE_DESCRIPTOR)).WillOnce(Return(1));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(FILE_DESCRIPTOR, _, 1));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  driver.Stop();
}

/*! \test UartImuDriver: poll for new bytes available at start. */
TEST_F(UartImuDriverTest, UartImuPollForByteavailableAtStart) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(FILE_DESCRIPTOR));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(FILE_DESCRIPTOR))
      .WillOnce(Return(0))
      .WillRepeatedly(Return(1));
  EXPECT_CALL(os_abstraction_layer_, TimeStampNow()).Times(AtLeast(0));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(FILE_DESCRIPTOR, _, 1)).Times(AtLeast(1));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  driver.Stop();
}

/*! \brief Assemble a complete frame based on given imu message data.*/
std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> AssembleFrame(
    const std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION>& imu_data) {
  std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> frame{};
  frame[0] = uart_imu::START_BYTE;
  std::copy(imu_data.begin(), imu_data.end(), frame.begin() + 1);
  return frame;
}

/*! \brief Create some expected ImuData
 * \param expected_timestamp The expected timestamp for the ImuData
 */
messages::ImuData CreateExpectedImuData(const std::chrono::nanoseconds& expected_timestamp) {
  messages::ImuData expected_imu_data{};
  expected_imu_data.a_x = 0.1F;
  expected_imu_data.a_y = 0.2F;
  expected_imu_data.a_z = 0.3F;
  expected_imu_data.omega_x = 0.4F;
  expected_imu_data.omega_y = 0.5F;
  expected_imu_data.omega_z = 0.6F;
  expected_imu_data.temperature = 25.4F;
  expected_imu_data.timestamp = expected_timestamp;
  return expected_imu_data;
}

/*! \brief Check if two ImuData messages are equal
 * \param expected_imu_data The expected ImuData message
 * \param imu_data The ImuData message to check
 */
void ExpectMesssagesAreEqual(const messages::ImuData& expected_imu_data, const messages::ImuData& imu_data) {
  EXPECT_NEAR(expected_imu_data.a_x, imu_data.a_x, uart_imu::LSB_SENSITIVITY_ACCELERATION)
      << "IMU data a_x should not differ more then the LSB sensitivity" << expected_imu_data.a_x
      << " vs: " << imu_data.a_x << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_ACCELERATION;
  EXPECT_NEAR(expected_imu_data.a_y, imu_data.a_y, uart_imu::LSB_SENSITIVITY_ACCELERATION)
      << "IMU data a_y should not differ more then the LSB sensitivity" << expected_imu_data.a_y
      << " vs: " << imu_data.a_y << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_ACCELERATION;
  EXPECT_NEAR(expected_imu_data.a_z, imu_data.a_z, uart_imu::LSB_SENSITIVITY_ACCELERATION)
      << "IMU data a_y should not differ more then the LSB sensitivity" << expected_imu_data.a_z
      << " vs: " << imu_data.a_z << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_ACCELERATION;

  EXPECT_NEAR(expected_imu_data.omega_x, imu_data.omega_x, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY)
      << "IMU data omega_x should not differ more then the LSB sensitivity" << expected_imu_data.omega_x
      << " vs: " << imu_data.omega_x << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY;
  EXPECT_NEAR(expected_imu_data.omega_y, imu_data.omega_y, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY)
      << "IMU data omega_y should not differ more then the LSB sensitivity" << expected_imu_data.omega_y
      << " vs: " << imu_data.omega_y << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY;
  EXPECT_NEAR(expected_imu_data.omega_z, imu_data.omega_z, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY)
      << "IMU data omega_z should not differ more then the LSB sensitivity" << expected_imu_data.omega_z
      << " vs: " << imu_data.omega_z << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY;

  EXPECT_NEAR(expected_imu_data.temperature, imu_data.temperature, uart_imu::LSB_SENSITIVITY_TEMPERATURE)
      << "IMU data temperature should not differ more then the LSB sensitivity" << expected_imu_data.temperature
      << " vs: " << imu_data.temperature << " LSB sens.: " << uart_imu::LSB_SENSITIVITY_TEMPERATURE;

  EXPECT_EQ(expected_imu_data.timestamp, imu_data.timestamp) << "IMU data timestamp should be the same";
}

/*! \test UartImuDriver: create messages from valid byte stream.
 * Sys-Req: TS1
 */
TEST_F(UartImuDriverTestReadingFile, UartImuReadBytesFromFile) {
  const auto expected_timestamp{std::chrono::nanoseconds{123456789}};
  messages::ImuData expected_imu_data = CreateExpectedImuData(expected_timestamp);

  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(FILE_DESCRIPTOR));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(FILE_DESCRIPTOR))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillRepeatedly(Return(static_cast<int>(uart_imu::TOTAL_NUMBER_OF_BYTES)));
  EXPECT_CALL(os_abstraction_layer_, TimeStampNow()).Times(AtLeast(1)).WillRepeatedly(Return(expected_timestamp));
  EXPECT_CALL(os_abstraction_layer_,
              ReadFromFile(FILE_DESCRIPTOR, _, static_cast<int>(uart_imu::TOTAL_NUMBER_OF_BYTES)))
      .Times(AtLeast(1));

  os_abstraction_layer_.SetBytesToReturn(AssembleFrame(serializer::uart::Serialize(expected_imu_data)));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::DURATION_BETWEEN_TWO_START_BYTES);
  EXPECT_TRUE(imu_driver_context_.GetStatus() == messages::ImuDriverStatus::OK) << "Status should be OK";

  driver.Stop();
  std::lock_guard<std::mutex> lock(imu_driver_context_.queue_mutex);

  EXPECT_TRUE(!imu_driver_context_.imu_data_queue.empty()) << "IMU data should be in the queue";
  ExpectMesssagesAreEqual(expected_imu_data, imu_driver_context_.imu_data_queue.front());
}

/*! \test UartImuDriver: shall not create messages from invalid byte stream.
 * Sys-Req: TS7
 */
TEST_F(UartImuDriverTestReadingFile, UartImuSetStatusToNoDataWhenNoMoreData) {
  const auto expected_timestamp{std::chrono::nanoseconds{123456789}};
  const auto far_future_timestamp{2 * expected_timestamp};
  messages::ImuData expected_imu_data = CreateExpectedImuData(expected_timestamp);

  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(FILE_DESCRIPTOR));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(FILE_DESCRIPTOR))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillOnce(Return(static_cast<int>(uart_imu::TOTAL_NUMBER_OF_BYTES)))
      .WillRepeatedly(Return(0));
  EXPECT_CALL(os_abstraction_layer_, TimeStampNow()).Times(AtLeast(1));
  EXPECT_CALL(os_abstraction_layer_,
              ReadFromFile(FILE_DESCRIPTOR, _, static_cast<int>(uart_imu::TOTAL_NUMBER_OF_BYTES)))
      .Times(AtLeast(1));

  os_abstraction_layer_.SetBytesToReturn(AssembleFrame(serializer::uart::Serialize(expected_imu_data)));
  os_abstraction_layer_.SetTimeIncrement(std::chrono::duration_cast<std::chrono::nanoseconds>(uart_imu::TIMEOUT));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(50ms);
  EXPECT_TRUE(imu_driver_context_.GetStatus() == messages::ImuDriverStatus::NO_DATA) << "Status should be NO_DATA";
  driver.Stop();
  EXPECT_TRUE(imu_driver_context_.GetStatus() == messages::ImuDriverStatus::NO_DATA)
      << "Status should set back to NO_DATA";
}

/*! \test UartImuDriver: shall not create empty messages.*/
TEST(PushMesagesInQueue, EmptyStream) {
  uart_imu::DriverContext context{};
  std::vector<std::byte> bytes_stream_from_imu{};
  uart_imu::PushMesagesInQueue(bytes_stream_from_imu, context, std::chrono::nanoseconds{0});
  EXPECT_TRUE(context.imu_data_queue.empty()) << "Queue should be empty when stream is empty";
}

/*! \test UartImuDriver: shall not convert partial buffer into messages.*/
TEST(PushMesagesInQueue, PartialMessage) {
  uart_imu::DriverContext context{};
  std::vector<std::byte> bytes_stream_from_imu{uart_imu::START_BYTE, std::byte{0x0d}, std::byte{0x0c}, std::byte{0x0b}};
  uart_imu::PushMesagesInQueue(bytes_stream_from_imu, context, std::chrono::nanoseconds{0});
  EXPECT_TRUE(context.imu_data_queue.empty()) << "Queue should be empty when stream is empty";
}

/*! \test UartImuDriver: shall create messages from the read bytes stream.*/
TEST(PushMesagesInQueue, FullMessageInStream) {
  const auto expected_timestamp{std::chrono::nanoseconds{123456789}};
  const auto expected_imu_data = CreateExpectedImuData(expected_timestamp);
  const auto full_frame = AssembleFrame(serializer::uart::Serialize(expected_imu_data));
  std::vector<std::byte> bytes_stream_from_imu{std::begin(full_frame), std::end(full_frame)};

  uart_imu::DriverContext context{};
  uart_imu::PushMesagesInQueue(bytes_stream_from_imu, context, expected_timestamp);

  EXPECT_EQ(1U, context.imu_data_queue.size()) << "Queue should have one element";
  ExpectMesssagesAreEqual(expected_imu_data, context.imu_data_queue.front());
}

/*! \test UartImuDriver: shall create multiple messages if multiple content in the read bytes stream.*/
TEST(PushMesagesInQueue, TwoMessageInStream) {
  const auto expected_timestamp{std::chrono::nanoseconds{123456789}};
  const auto expected_imu_data = CreateExpectedImuData(expected_timestamp);
  const auto full_frame = AssembleFrame(serializer::uart::Serialize(expected_imu_data));
  std::vector<std::byte> bytes_stream_from_imu{std::begin(full_frame), std::end(full_frame)};
  bytes_stream_from_imu.insert(std::end(bytes_stream_from_imu), std::begin(full_frame), std::end(full_frame));

  ASSERT_EQ(2 * uart_imu::TOTAL_NUMBER_OF_BYTES, bytes_stream_from_imu.size()) << "Stream should have two full frames";

  uart_imu::DriverContext context{};
  uart_imu::PushMesagesInQueue(bytes_stream_from_imu, context, expected_timestamp);

  EXPECT_EQ(2U, context.imu_data_queue.size()) << "Queue should have two element";
  ExpectMesssagesAreEqual(expected_imu_data, context.imu_data_queue.front());
  ExpectMesssagesAreEqual(expected_imu_data, context.imu_data_queue.back());
}
