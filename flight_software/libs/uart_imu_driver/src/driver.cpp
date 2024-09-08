// Copyright 2024 <Maxime Haselbauer>
#include <messages/imu_data.h>
#include <serializer/serializer.h>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver.h>

#include <algorithm>
#include <thread>
namespace uart_imu {

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

bool StartByteFound(const std::vector<std::byte> bytes_stream_from_imu) {
  if (bytes_stream_from_imu.empty()) {
    return false;
  }
  return bytes_stream_from_imu.front() == uart_imu::START_BYTE;
}

void CleanStreamUpToStartByte(std::vector<std::byte>& bytes_stream_from_imu) {
  auto start_byte_iterator =
      std::find(bytes_stream_from_imu.begin(), bytes_stream_from_imu.end(), uart_imu::START_BYTE);
  bytes_stream_from_imu.erase(bytes_stream_from_imu.begin(), start_byte_iterator);
}

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
  driver_must_stop_ = true;
  if (driver_thread_.joinable()) {
    driver_thread_.join();
  }
}

void Driver::FlushTheDeviceFile() const {
  ReadBytesFromDevice();
}

std::vector<std::byte> Driver::ReadBytesFromDevice() const {
  const int bytes_available = os_layer_.ByteAvailableToRead(file_descriptor_);
  if (bytes_available < 1) {
    return {};
  }
  std::vector<std::byte> byte_read_from_imu(bytes_available);
  os_layer_.ReadFromFile(file_descriptor_, reinterpret_cast<char*>(byte_read_from_imu.data()), bytes_available);
  return byte_read_from_imu;
}

void Driver::Run() {
  driver_context_.SetStatus(messages::ImuDriverStatus::OK);
  FlushTheDeviceFile();

  std::vector<std::byte> bytes_stream_from_imu{};

  while (!driver_must_stop_) {
    while (!StartByteFound(bytes_stream_from_imu)) {
      bytes_stream_from_imu = ReadBytesFromDevice();
      CleanStreamUpToStartByte(bytes_stream_from_imu);
      if (driver_must_stop_) {
        return;
      }
      std::this_thread::sleep_for(uart_imu::FRAME_DURATION_US);
    }
    const auto timestamp = os_layer_.TimeStampNow();

    // First bit has been received, now sleep until all the bytes are transferred.
    std::this_thread::sleep_for(uart_imu::MESSAGE_DURATION_US);

    auto byte_read_from_imu = ReadBytesFromDevice();
    bytes_stream_from_imu.insert(bytes_stream_from_imu.end(), byte_read_from_imu.begin(), byte_read_from_imu.end());
    CleanStreamUpToStartByte(bytes_stream_from_imu);
    if (IsThereAtLeastOneMessage(bytes_stream_from_imu)) {
      PushMesagesInQueue(bytes_stream_from_imu, driver_context_, timestamp);
    }
    // Message has been received and pushe. Now sleep until the start of next message.
    std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  }
}

}  // namespace uart_imu
