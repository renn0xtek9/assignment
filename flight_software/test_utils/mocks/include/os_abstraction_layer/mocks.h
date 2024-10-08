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

/*! \brief Basic mock class for the OsAbstractionLayerInterface.*/
class MockOsAbstractionLayer : public OsAbstractionLayerInterface {
 public:
  MOCK_METHOD(int, OpenDeviceFile, (const std::string& device_file_path), (const, override)); /**< Mock OpenDeviceFile*/
  MOCK_METHOD(int, CloseDeviceFile, (int file_descriptor), (const, override)); /**< Mock CloseDeviceFile*/
  MOCK_METHOD(std::size_t,
              Write,
              (const int& file_descriptor, const void* data, const std::size_t& size),
              (const, override)); /**< Mock Write*/
  MOCK_METHOD(int,
              ByteAvailableToRead,
              (const int& file_descriptor),
              (const, override));                                             /**< Mock ByteAvailableToRead*/
  MOCK_METHOD(std::chrono::nanoseconds, TimeStampNow, (), (const, override)); /**< Mock TimeStampNow*/
  MOCK_METHOD(void,
              ReadFromFile,
              (const int& file_descriptor, char* ptr, const std::size_t& size),
              (const, override)); /**< Mock ReadFromFile*/
};

/*! \brief Mock class of OsAbstractionLayerInterface to simulate reading from a file.*/
class MockReadingFile : public OsAbstractionLayerInterface {
 public:
  /*! \brief Define that data that the mock will return via the OS abstraction layer.*/
  void SetBytesToReturn(const std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES>& bytes) {
    bytes_to_return_ = bytes;
  }

  /*! \brief Define the time increment (between every call time TimeStamp)*/
  void SetTimeIncrement(const std::chrono::nanoseconds& time_increment) {
    time_increment_ = time_increment;
  }

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

    ON_CALL(*this, TimeStampNow).WillByDefault([this]() {
      current_time_ += time_increment_;
      return current_time_;
    });
  }
  MOCK_METHOD(int, OpenDeviceFile, (const std::string& device_file_path), (const, override)); /**< Mock OpenDeviceFile*/
  MOCK_METHOD(int, CloseDeviceFile, (int file_descriptor), (const, override)); /**< Mock CloseDeviceFile*/
  MOCK_METHOD(std::size_t,
              Write,
              (const int& file_descriptor, const void* data, const std::size_t& size),
              (const, override)); /**< Mock Write*/
  MOCK_METHOD(int,
              ByteAvailableToRead,
              (const int& file_descriptor),
              (const, override));                                             /**< Mock ByteAvailableToRead*/
  MOCK_METHOD(std::chrono::nanoseconds, TimeStampNow, (), (const, override)); /**< Mock TimeStampNow*/
  MOCK_METHOD(void,
              ReadFromFile,
              (const int& file_descriptor, char* ptr, const std::size_t& size),
              (const, override)); /**< Mock ReadFromFile*/

 private:
  std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> bytes_to_return_{};
  std::size_t index_of_first_byte_{};
  std::chrono::nanoseconds current_time_{};
  std::chrono::nanoseconds time_increment_{};
};

}  // namespace OsAbstractionLayer

#endif  // MOCKS_H
