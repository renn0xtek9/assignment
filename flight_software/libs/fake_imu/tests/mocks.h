// Copyright 2024 <Maxime Haselbauer>
#ifndef MOCKS_H
#define MOCKS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <os_abstraction_layer/os_abstraction_layer_interface.h>

#include <functional>
#include <string>

namespace OsAbstractionLayer {

class MockOsAbstractionLayer : public OsAbstractionLayerInterface {
 public:
  MOCK_METHOD(bool, CheckDeviceFileExists, (const std::string& device_file_path), (const, override));
  MOCK_METHOD(int, OpenDeviceFile, (const std::string& device_file_path), (const, override));
  MOCK_METHOD(int, CloseDeviceFile, (int file_descriptor), (const, override));
  MOCK_METHOD(std::size_t,
              Write,
              (const int& file_descriptor, const void* data, const std::size_t& size),
              (const, override));
  MOCK_METHOD(int, ByteAvailableToRead, (const int& file_descriptor), (const, override));
  MOCK_METHOD(std::chrono::nanoseconds, TimeStampNow, (), (const, override));
  MOCK_METHOD(void, ReadFromFile, (const int& file_descriptor, char* ptr, const std::size_t& size), (const, override));
  MOCK_METHOD(void, TruncateFile, (const int& file_descriptor), (const, override));
};

}  // namespace OsAbstractionLayer

#endif  // MOCKS_H
