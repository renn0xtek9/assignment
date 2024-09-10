// Copyright 2024 <Maxime Haselbauer>
#include <communication_protocols/uart_imu.h>
#include <display_messages/bytes.h>
#include <display_messages/imu_information.h>
#include <fake_imu/fake_imu.h>
#include <messages/imu_data.h>
#include <os_abstraction_layer/os_abstraction_layer_interface.h>
#include <serializer/serializer.h>

#include <array>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

messages::ImuData GenerateFakeImuData() {
  messages::ImuData data{};
  data.a_x = 0.1F;
  data.a_y = 0.2F;
  data.a_z = 0.3F;
  data.omega_x = 0.4F;
  data.omega_y = 0.5F;
  data.omega_z = 0.5F;
  data.temperature = 25.6F;
  data.timestamp = std::chrono::nanoseconds{std::chrono::system_clock::now().time_since_epoch()};
  return data;
}

FakeImu::FakeImu(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abstraction_layer)
    : os_layer_{os_abstraction_layer} {
}

void FakeImu::SimulateNormalOperation(const std::string& device_file_path,
                                      const std::chrono::milliseconds& duration_ms) {
  file_descriptor_ = os_layer_.OpenDeviceFile(device_file_path);
  const auto start_timestamp = std::chrono::high_resolution_clock::now();
  auto time_since_start = std::chrono::high_resolution_clock::now() - start_timestamp;
  while ((file_descriptor_ > 0) && ((std::chrono::high_resolution_clock::now() - start_timestamp) < duration_ms)) {
    const auto start_of_next_message =
        std::chrono::high_resolution_clock::now() + uart_imu::DURATION_BETWEEN_TWO_START_BYTES;
    SendImuData(GenerateFakeImuData());
    std::this_thread::sleep_until(start_of_next_message);
  }
  if (file_descriptor_ > 0) {
    os_layer_.CloseDeviceFile(file_descriptor_);
    file_descriptor_ = -1;
  }
  return;
}

void FakeImu::SendByte(const std::byte& byte) {
  const auto byte_written = os_layer_.Write(file_descriptor_, &byte, 1U);
  if (byte_written != 1) {
    std::cerr << "Failed to write byte to device: " << strerror(errno) << "\n";
    os_layer_.CloseDeviceFile(file_descriptor_);
    file_descriptor_ = -1;
    return;
  }
  std::cout << byte << " ";
}

void FakeImu::SendImuData(const messages::ImuData& data) {
  std::cout << std::endl << "Sending IMU data: " << data << std::endl;
  std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> payload{serializer::uart::Serialize(data)};
  SendByte(uart_imu::START_BYTE);
  for (const auto& byte : payload) {
    if (file_descriptor_ < 1) {
      break;
    }
    SendByte(byte);
  }
  std::cout << std::endl;
}
