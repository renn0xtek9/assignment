// Copyright 2024 <Maxime Haselbauer>
#ifndef MOCKS_H
#define MOCKS_H

#include <device_handler/os_abstraction_layer_interface.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>
#include <string>

namespace OsAbstractionLayer {

class MockOsAbstractionLayer : public OsAbstractionLayerInterface {
 public:
  MOCK_METHOD(bool, CheckDeviceFileExists, (const std::string& device_file_path), (const, override));
  MOCK_METHOD(int, OpenDeviceFile, (const std::string& device_file_path), (const, override));
  MOCK_METHOD(int, CloseDeviceFile, (int file_descriptor), (const, override));
};

}  // namespace OsAbstractionLayer

#endif  // MOCKS_H
