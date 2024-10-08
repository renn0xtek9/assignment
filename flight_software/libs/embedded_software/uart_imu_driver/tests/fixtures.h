// Copyright 2024 <Maxime Haselbauer>
#ifndef FIXTURES_H
#define FIXTURES_H
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <os_abstraction_layer/mocks.h>
#include <uart_imu_driver/driver_context.h>
// #include "mocks.h"

/*! \brief Fixture for basic tests of the UartImuDriver*/
class UartImuDriverTest : public ::testing::Test {
 protected:
  OsAbstractionLayer::MockOsAbstractionLayer
      os_abstraction_layer_{};                   /**< Operating System abstraction layer interface.*/
  uart_imu::DriverContext imu_driver_context_{}; /**< IMU driver shared memory context.*/
};

/*! \brief Fixture for tests the UartImuDriver where we expect to read some data*/
class UartImuDriverTestReadingFile : public UartImuDriverTest {
 protected:
  OsAbstractionLayer::MockReadingFile os_abstraction_layer_{}; /**< Operating System abstraction layer interface.*/
  uart_imu::DriverContext imu_driver_context_{};               /**< IMU driver shared memory context.*/
};

#endif  // FIXTURES_H
