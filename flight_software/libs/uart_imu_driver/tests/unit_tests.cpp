// Copyright 2024 <Maxime Haselbauer>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>
#include <serializer/serializer.h>
#include <uart_imu/uart_imu.h>
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
  EXPECT_EQ(messages::ImuDriverStatus::NO_DATA, imu_driver_context_.GetStatus()) << "Initial status should be NO_DATA";

  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  EXPECT_EQ(messages::ImuDriverStatus::OK, imu_driver_context_.GetStatus()) << "Status should be OK after start";
  driver.Stop();
}

/*! \test UartImuDriver : flush the device file at start.*/
TEST_F(UartImuDriverTest, UartImuFlushDeviceFileAtStart) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(42));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(42)).WillOnce(Return(1));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(42, _, 1));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  driver.Stop();
}

/*! \test UartImuDriver: poll for new bytes available at start. */
TEST_F(UartImuDriverTest, UartImuPollForByteavailableAtStart) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(42));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(42)).WillOnce(Return(0)).WillRepeatedly(Return(1));
  EXPECT_CALL(os_abstraction_layer_, TimeStampNow()).Times(AtLeast(1));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(42, _, 1)).Times(AtLeast(1));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  driver.Stop();
}

std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> AssembleFrame(
    const std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION>& imu_data) {
  std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> frame{};
  frame[0] = uart_imu::START_BYTE;
  std::copy(imu_data.begin(), imu_data.end(), frame.begin() + 1);
  return frame;
}

/*! \test UartImuDriver: intake bytes comming from device file. */
TEST_F(UartImuDriverTestReadingFile, UartImuReadBytesFromFile) {
  const auto expected_timestamp{std::chrono::nanoseconds{123456789}};
  messages::ImuData expected_imu_data{};
  expected_imu_data.a_x = 0.1F;
  expected_imu_data.a_y = 0.2F;
  expected_imu_data.a_z = 0.3F;
  expected_imu_data.omega_x = 0.4F;
  expected_imu_data.omega_y = 0.5F;
  expected_imu_data.omega_z = 0.6F;
  expected_imu_data.temperature = 25.4F;
  expected_imu_data.timestamp = expected_timestamp;

  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(42));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(42))
      .WillOnce(Return(0))
      .WillOnce(Return(0))
      .WillRepeatedly(Return(static_cast<int>(uart_imu::TOTAL_NUMBER_OF_BYTES)));
  EXPECT_CALL(os_abstraction_layer_, TimeStampNow()).Times(AtLeast(1)).WillRepeatedly(Return(expected_timestamp));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(42, _, static_cast<int>(uart_imu::TOTAL_NUMBER_OF_BYTES)));

  os_abstraction_layer_.SetBytesToReturn(AssembleFrame(serializer::uart::Serialize(expected_imu_data)));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  driver.Stop();
  std::lock_guard<std::mutex> lock(imu_driver_context_.queue_mutex);

  EXPECT_TRUE(!imu_driver_context_.imu_data_queue.empty()) << "IMU data should be in the queue";
  const auto imu_data = imu_driver_context_.imu_data_queue.front();
  EXPECT_NEAR(expected_imu_data.a_x, imu_data.a_x, uart_imu::LSB_SENSITIVITY_ACCELERATION)
      << "IMU data a_x should not differ more then the LSB sensitivity";
  EXPECT_NEAR(expected_imu_data.a_y, imu_data.a_y, uart_imu::LSB_SENSITIVITY_ACCELERATION)
      << "IMU data a_y should not differ more then the LSB sensitivity";
  EXPECT_NEAR(expected_imu_data.a_z, imu_data.a_z, uart_imu::LSB_SENSITIVITY_ACCELERATION)
      << "IMU data a_y should not differ more then the LSB sensitivity";

  EXPECT_NEAR(expected_imu_data.omega_x, imu_data.omega_x, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY)
      << "IMU data omega_x should not differ more then the LSB sensitivity";
  EXPECT_NEAR(expected_imu_data.omega_y, imu_data.omega_y, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY)
      << "IMU data omega_y should not differ more then the LSB sensitivity";
  EXPECT_NEAR(expected_imu_data.omega_z, imu_data.omega_z, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY)
      << "IMU data omega_z should not differ more then the LSB sensitivity";

  EXPECT_NEAR(expected_imu_data.temperature, imu_data.temperature, uart_imu::LSB_SENSITIVITY_TEMPERATURE)
      << "IMU data temperature should not differ more then the LSB sensitivity";

  EXPECT_EQ(expected_imu_data.timestamp, imu_data.timestamp) << "IMU data timestamp should be the same";
}
