// Copyright 2024 <Maxime Haselbauer>

#include <fake_imu/fake_imu.h>
#include <fake_imu/software_in_the_loop_constant.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fixtures.h"

using ::testing::_;
using ::testing::Return;

/*! \test FakeImu: shall be initialized with a simple file descriptor*/
TEST_F(FakeImuTest, FakeImuInitialization) {
  FakeImu fake_imu{os_abstraction_layer_};
}

/*! \test FakeImu: Simulating normal operation shall open a specified device file*/
TEST_F(FakeImuTest, FakeImuSimulateNormalOperation) {
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile(std::string{kFakeImuSilDeviceFilePath})).WillOnce(Return(42));

  FakeImu fake_imu{os_abstraction_layer_};
  fake_imu.SimulateNormalOperation(std::string{kFakeImuSilDeviceFilePath});
}
