// Copyright 2024 <Maxime Haselbauer>
/*! \file unit_test.cpp
 * \brief unit tests of device handler library
 */
#include <device_handler/device_handler.h>
#include <device_handler/os_abstraction_layer.h>
#include <fcntl.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fixtures.h"

using ::testing::_;
using ::testing::Return;

/*! \test HandleDevice Returns true if file is not found
 */
TEST_F(HandleDeviceFileTest, DeviceFileFoundAndOpenedReturnFalse) {
  EXPECT_CALL(os_abstraction_layer_, CheckDeviceFileExists("existing_file")).WillOnce(Return(true));
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("existing_file")).WillOnce(Return(10));
  EXPECT_CALL(os_abstraction_layer_, CloseDeviceFile(10)).WillOnce(Return(0));

  DeviceHandler device_handler(os_abstraction_layer_);
  EXPECT_FALSE(device_handler.HandleDevice("existing_file"));
}

/**/

/*! \test HandleDevice Returns false if file is found and we can open it
 */
TEST_F(HandleDeviceFileTest, DeviceFileDoesNotExistReturnTrue) {
  EXPECT_CALL(os_abstraction_layer_, CheckDeviceFileExists("non_existing_file")).WillOnce(Return(false));

  DeviceHandler device_handler(os_abstraction_layer_);
  EXPECT_TRUE(device_handler.HandleDevice("non_existing_file"));
}

/*! \test HandleDevice Returns true if file is found and we cannot open it
 */
TEST_F(HandleDeviceFileTest, DeviceFileFoundButCannotOpenReturnTrue) {
  EXPECT_CALL(os_abstraction_layer_, CheckDeviceFileExists("existing_file")).WillOnce(Return(true));
  EXPECT_CALL(os_abstraction_layer_, OpenDeviceFile("existing_file")).WillOnce(Return(-1));

  DeviceHandler device_handler(os_abstraction_layer_);
  EXPECT_FALSE(device_handler.HandleDevice("existing_file"));
}

/* \test Can safely instantiate and destroy OsAbstractionLayer
 */
TEST(OsAbstractionLayerTest, CanInstantiateOsAbstractionLayer) {
  { OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{}; }
}
