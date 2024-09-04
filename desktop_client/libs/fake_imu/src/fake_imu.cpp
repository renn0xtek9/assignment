// Copyright 2024 <Maxime Haselbauer>
#include <fake_imu/fake_imu.h>
#include <os_abstraction_layer/os_abstraction_layer.h>

FakeImu::FakeImu(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abstraction_layer)
    : os_layer_{os_abstraction_layer} {
}

void FakeImu::SimulateNormalOperation(const std::string& device_file_path) {
  os_layer_.OpenDeviceFile(device_file_path);
  std::cout << &os_layer_ << std::endl;
}

// #include <fcntl.h>
// #include <unistd.h>

// #include <array>
// #include <cstddef>
// #include <cstring>
// #include <iostream>
// #include <thread>

// constexpr std::uint16_t IMU_MEASUREMENT_PER_SECONDS{125U};
// constexpr std::uint32_t SLEEP_TIME_BETWEEN_MEASUREMENTS_MS{
//     static_cast<std::uint32_t>(static_cast<double>(1000) / static_cast<double>(IMU_MEASUREMENT_PER_SECONDS))};
// struct ImuData {
//   float x{0.1F};
//   float y{0.2F};
//   float z{0.3F};
// };

// std::ostream& operator<<(std::ostream& out, const ImuData& data) {
//   out << "x: " << data.x << " y: " << data.y << " z: " << data.z << std::endl;
//   return out;
// }

// int open_serial_file(const std::string& device_file) {
//   int file_descriptor = open(device_file.c_str(), O_RDWR | O_NOCTTY);
//   if (file_descriptor < 0) {
//     std::cerr << "Failed to open " << device_file << " : " << strerror(errno) << "\n ";
//     return 1;
//   }

//   std::cout << "Successfully opened device: " << device_file << std::endl;
//   return file_descriptor;
// }

// void send_byte(const int& file_descriptor, const std::byte byte) {
//   const auto byte_written = write(file_descriptor, &byte, 1U);
//   if (byte_written != 1U) {
//     std::cerr << "Failed to device: " << strerror(errno) << "\n";
//     close(file_descriptor);
//     return;
//   }
//   std::cout << "Sent byte: " << static_cast<int>(byte) << std::endl;
// }

// void send_data(const int& file_descriptor, const ImuData& data) {
//   std::cout << "Send IMU message: " << data;
//   std::array<std::byte, sizeof(data)> payload;
//   std::memcpy(payload.data(), &data, payload.size());
//   for (const auto& byte : payload) {
//     send_byte(file_descriptor, byte);
//   }
// }

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
