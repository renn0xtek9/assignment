// Copyright 2024 <Maxime Haselbauer>
#include <fake_imu/fake_imu.h>
#include <fake_imu/software_in_the_loop_constant.h>
#include <os_abstraction_layer/os_abstraction_layer.h>

#include <array>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

FakeImu::FakeImu(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abstraction_layer)
    : os_layer_{os_abstraction_layer} {
}

void FakeImu::SimulateNormalOperation(const std::string& device_file_path,
                                      const std::chrono::milliseconds& duration_ms) {
  file_descriptor_ = os_layer_.OpenDeviceFile(device_file_path);
  const auto start_timestamp = std::chrono::high_resolution_clock::now();
  while ((file_descriptor_ > 0) && (std::chrono::high_resolution_clock::now() - start_timestamp < duration_ms)) {
    SendImuData(ImuData{});
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_BETWEEN_MEASUREMENTS_MS));
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
  std::cout << "Sent byte: " << static_cast<int>(byte) << std::endl;
}

void FakeImu::SendImuData(const ImuData& data) {
  std::array<std::byte, sizeof(data)> payload{};
  std::memcpy(payload.data(), &data, payload.size());
  for (const auto& byte : payload) {
    if (file_descriptor_ < 1) {
      break;
    }
    SendByte(byte);
  }
}

// #include <fcntl.h>
// #include <unistd.h>

// #include <array>
// #include <cstddef>
// #include <cstring>
// #include <iostream>
// #include <thread>

// void simulate_functionning_imu(const int& file_descriptor) {
//   while (true) {
//     send_data(file_descriptor, ImuData{});
//     std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_BETWEEN_MEASUREMENTS_MS));
//   }
// }

// int main() {
//   std::cout << "Distance between two measurements: " << SLEEP_TIME_BETWEEN_MEASUREMENTS_MS << std::endl;
//   const std::string device_file = "/tmp/vserial1";
//   const auto file_descriptor = open_serial_file(device_file);
//   simulate_functionning_imu(file_descriptor);
//   close(file_descriptor);
//   return 0;
// }
