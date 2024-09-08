// Copyright 2024 <Maxime Haselbauer>
#include <messages/imu_data.h>
#include <serializer/serializer.h>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver.h>

#include <algorithm>
#include <thread>
namespace uart_imu {
Driver::Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
               DriverContext& driver_context,
               const std::string device_file_path)
    : os_layer_{os_abastrction_layer}, driver_context_{driver_context}, device_file_path_{device_file_path} {
}

void Driver::Start() {
  printf("Starting IMU driver.\n");
  file_descriptor_ = os_layer_.OpenDeviceFile(device_file_path_);
  if (file_descriptor_ < 0) {
    std::cout << "Driver could not open device file: " << device_file_path_ << std::endl;
    Stop();
    return;
  }
  driver_thread_ = std::thread(&Driver::Run, this);
}
void Driver::Stop() {
  printf("Stopping IMU driver.\n");
  driver_must_stop = true;
  if (driver_thread_.joinable()) {
    driver_thread_.join();
  }
}

bool IsThereAtLeastOneMessage(const std::vector<std::byte>& bytes_stream_from_imu) {
  return bytes_stream_from_imu.size() >= uart_imu::TOTAL_NUMBER_OF_BYTES;
}

void PushMesagesInQueue(std::vector<std::byte>& bytes_stream_from_imu,
                        DriverContext& driver_context,
                        const std::chrono::nanoseconds& timestamp) {
  std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> message_byte_array{};
  std::copy(bytes_stream_from_imu.begin() + uart_imu::NUMBER_START_BYTE,
            bytes_stream_from_imu.begin() + uart_imu::TOTAL_NUMBER_OF_BYTES, message_byte_array.begin());

  auto imu_message = serializer::uart::Deserialize(message_byte_array);
  imu_message.timestamp = timestamp;
  driver_context.PushData(imu_message);
  bytes_stream_from_imu.erase(bytes_stream_from_imu.begin(),
                              bytes_stream_from_imu.begin() + uart_imu::TOTAL_NUMBER_OF_BYTES);
}

void FlushTheDeviceFile(const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer, int file_descriptor) {
  const int bytes_available = os_layer.ByteAvailableToRead(file_descriptor);
  if (bytes_available > 0) {
    std::vector<std::byte> byte_read_from_imu(bytes_available);
    os_layer.ReadFromFile(file_descriptor, reinterpret_cast<char*>(byte_read_from_imu.data()), bytes_available);
  }
}

void Driver::Run() {
  printf("IMU driver is running.\n");
  driver_context_.SetStatus(messages::ImuDriverStatus::OK);
  FlushTheDeviceFile(os_layer_, file_descriptor_);

  std::vector<std::byte> bytes_stream_from_imu{};
  while (!driver_must_stop) {
    int bytes_available = os_layer_.ByteAvailableToRead(file_descriptor_);
    while (bytes_available < 1) {
      std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
      bytes_available = os_layer_.ByteAvailableToRead(file_descriptor_);
      if (driver_must_stop) {
        return;
      }
    }
    const auto timestamp = os_layer_.TimeStampNow();

    std::vector<std::byte> byte_read_from_imu(bytes_available);
    os_layer_.ReadFromFile(file_descriptor_, reinterpret_cast<char*>(byte_read_from_imu.data()), bytes_available);
    bytes_stream_from_imu.insert(bytes_stream_from_imu.end(), byte_read_from_imu.begin(), byte_read_from_imu.end());
    auto start_byte_iterator =
        std::find(bytes_stream_from_imu.begin(), bytes_stream_from_imu.end(), uart_imu::START_BYTE);
    bytes_stream_from_imu.erase(bytes_stream_from_imu.begin(), start_byte_iterator);
    if (IsThereAtLeastOneMessage(bytes_stream_from_imu)) {
      PushMesagesInQueue(bytes_stream_from_imu, driver_context_, timestamp);
    }
    std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  }
}

}  // namespace uart_imu
// Compare this snippet from flight_software/libs/uart_imu_driver/src/driver_context.cpp:
