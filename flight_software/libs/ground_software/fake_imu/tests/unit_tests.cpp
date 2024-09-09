// Copyright 2024 <Maxime Haselbauer>

#include <communication_protocols/uart_imu.h>
#include <fake_imu/fake_imu.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <software_in_the_loop/sil_configuration.h>

#include <chrono>

#include "fixtures.h"

using namespace std::chrono_literals;  // NOLINT

using ::testing::_;
using ::testing::Return;

/*! \test FakeImu: shall be initialized with a simple file descriptor.*/
TEST_F(FakeImuTest, FakeImuInitialization) {
  FakeImu fake_imu{os_abstraction_layer_};
}

/*! \test FakeImu: Simulating normal operation shall open a specified device file, write to it and close the file for a
 * given duration.*/
TEST_F(FakeImuTest, FakeImuSimulateNormalOperation) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile(std::string{kFakeImuSilDeviceFilePath})).WillOnce(Return(42));
  EXPECT_CALL(os_abstraction_layer_, Write(42, _, 1U)).WillRepeatedly(Return(1));
  EXPECT_CALL(os_abstraction_layer_, CloseDeviceFile(42)).WillOnce(Return(0));
  FakeImu fake_imu{os_abstraction_layer_};

  const auto start_timestamp = std::chrono::high_resolution_clock::now();
  // Simulate normal operation for 30ms
  fake_imu.SimulateNormalOperation(std::string{kFakeImuSilDeviceFilePath}, 30ms);
  const auto end_timestamp = std::chrono::high_resolution_clock::now();
  EXPECT_TRUE(end_timestamp - start_timestamp >= 30ms);
}

/*! \test FakeImu: Close the device file if it cannot write to it.*/
TEST_F(FakeImuTest, CloseDeviceFileIfCannotWriteToIt) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile(std::string{kFakeImuSilDeviceFilePath})).WillOnce(Return(42));
  EXPECT_CALL(os_abstraction_layer_, Write(42, _, 1U)).WillOnce(Return(0));
  EXPECT_CALL(os_abstraction_layer_, CloseDeviceFile(42)).WillOnce(Return(0));
  FakeImu fake_imu{os_abstraction_layer_};
  fake_imu.SimulateNormalOperation(std::string{kFakeImuSilDeviceFilePath}, 30ms);
}
