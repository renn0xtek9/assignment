// Copyright 2024 <Maxime Haselbauer>
#ifndef MOCKS_H
#define MOCKS_H

#include <communication_protocols/uart_imu.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <os_abstraction_layer/os_abstraction_layer_interface.h>

#include <algorithm>
#include <functional>
#include <string>

namespace OsAbstractionLayer {

class MockOsAbstractionLayer : public OsAbstractionLayerInterface {
 public:
  MOCK_METHOD(int, OpenDeviceFile, (const std::string& device_file_path), (const, override));
  MOCK_METHOD(int, CloseDeviceFile, (int file_descriptor), (const, override));
  MOCK_METHOD(std::size_t,
              Write,
              (const int& file_descriptor, const void* data, const std::size_t& size),
              (const, override));
  MOCK_METHOD(int, ByteAvailableToRead, (const int& file_descriptor), (const, override));
  MOCK_METHOD(std::chrono::nanoseconds, TimeStampNow, (), (const, override));
  MOCK_METHOD(void, ReadFromFile, (const int& file_descriptor, char* ptr, const std::size_t& size), (const, override));
};

class MockReadingFile : public OsAbstractionLayerInterface {
 public:
  MockReadingFile() {
    ON_CALL(*this, ReadFromFile).WillByDefault([this](const int&, char* ptr, const std::size_t& size) {
      EXPECT_TRUE(index_of_first_byte_ + size < bytes_to_return_.size() + 1U)
          << __FILE__ << ":" << __LINE__ << " index_of_first_byte: " << index_of_first_byte_ << " size: " << size
          << " bytes_to_return_.size(): " << bytes_to_return_.size();

      std::transform(bytes_to_return_.begin() + index_of_first_byte_,
                     bytes_to_return_.begin() + index_of_first_byte_ + size, ptr,
                     [](std::byte byte) { return static_cast<char>(byte); });
      index_of_first_byte_ += size;
      if (index_of_first_byte_ >= bytes_to_return_.size()) {
        index_of_first_byte_ = 0;
      }
    });
  }
  MOCK_METHOD(int, OpenDeviceFile, (const std::string& device_file_path), (const, override));
  MOCK_METHOD(int, CloseDeviceFile, (int file_descriptor), (const, override));
  MOCK_METHOD(std::size_t,
              Write,
              (const int& file_descriptor, const void* data, const std::size_t& size),
              (const, override));
  MOCK_METHOD(int, ByteAvailableToRead, (const int& file_descriptor), (const, override));
  MOCK_METHOD(std::chrono::nanoseconds, TimeStampNow, (), (const, override));
  MOCK_METHOD(void, ReadFromFile, (const int& file_descriptor, char* ptr, const std::size_t& size), (const, override));

  void SetBytesToReturn(const std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES>& bytes) {
    bytes_to_return_ = bytes;
  }

 private:
  std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> bytes_to_return_{};
  std::size_t index_of_first_byte_{};
};

}  // namespace OsAbstractionLayer

#endif  // MOCKS_H
