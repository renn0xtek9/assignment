// Copyright 2024 <Maxime Haselbauer>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver.h>
#include <uart_imu_driver/driver_context.h>

#include <chrono>
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

/*! \test UartImuDriver: poll for new bytes available at start. */
TEST_F(UartImuDriverTest, UartImuPollForByteavailableAtStart) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("some_device_file")).WillOnce(Return(42));
  EXPECT_CALL(os_abstraction_layer_, ByteAvailableToRead(42)).WillOnce(Return(0)).WillRepeatedly(Return(1));
  EXPECT_CALL(os_abstraction_layer_, ReadFromFile(42, _, 1));

  uart_imu::Driver driver(os_abstraction_layer_, imu_driver_context_, {"some_device_file"});
  driver.Start();
  std::this_thread::sleep_for(2 * uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  driver.Stop();
}
