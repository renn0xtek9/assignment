// Copyright 2024 <Maxime Haselbauer>
#ifndef FIXTURES_H
#define FIXTURES_H
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <uart_imu_driver/driver_context.h>

#include "mocks.h"

class UartImuDriverTest : public ::testing::Test {
 protected:
  OsAbstractionLayer::MockOsAbstractionLayer os_abstraction_layer_{};
  uart_imu::DriverContext imu_driver_context_{};
};

#endif  // FIXTURES_H
