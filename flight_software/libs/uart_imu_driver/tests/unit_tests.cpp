// Copyright 2024 <Maxime Haselbauer>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <messages/imu_data.h>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver.h>

#include <chrono>
#include <queue>

#include "fixtures.h"

using namespace std::chrono_literals;  // NOLINT

using ::testing::_;
using ::testing::Return;

/*! \test UartImuDriver: shall not throw when initialized.*/
TEST_F(UartImuDriverTest, FakeImuInitialization) {
  std::queue<messages::ImuData> imu_data_queue{};
  std::mutex queue_mutex{};
  uart_imu::Driver driver(os_abstraction_layer_, imu_data_queue, queue_mutex, {"some_device_file"});
}
