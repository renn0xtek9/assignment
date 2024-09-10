// Copyright 2024 <Maxime Haselbauer>
#include <communication_protocols/uart_imu.h>
#include <messages/imu_data.h>
#include <serializer/serializer.h>
#include <uart_imu_driver/driver.h>

#include <algorithm>
#include <thread>
namespace uart_imu {

/*! \brief Check if the byte stream contains at least one message.*/
bool IsThereAtLeastOneMessage(const std::vector<std::byte>& bytes_stream_from_imu) {
  return bytes_stream_from_imu.size() >= uart_imu::TOTAL_NUMBER_OF_BYTES;
}

/*! \brief Push new messages in queue.*/
void PushMesagesInQueue(std::vector<std::byte>& bytes_stream_from_imu,
                        DriverContext& driver_context,
                        const std::chrono::nanoseconds& timestamp) {
  while (!bytes_stream_from_imu.empty()) {
    if (bytes_stream_from_imu.size() < uart_imu::TOTAL_NUMBER_OF_BYTES) {
      return;
    }
    std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> message_byte_array{};
    std::copy(bytes_stream_from_imu.begin() + uart_imu::NUMBER_START_BYTE,
              bytes_stream_from_imu.begin() + uart_imu::TOTAL_NUMBER_OF_BYTES, message_byte_array.begin());

    auto imu_message = serializer::uart::Deserialize(message_byte_array);
    imu_message.timestamp = timestamp;
    driver_context.PushData(imu_message);
    bytes_stream_from_imu.erase(bytes_stream_from_imu.begin(),
                                bytes_stream_from_imu.begin() + uart_imu::TOTAL_NUMBER_OF_BYTES);
  }
}

/*! \brief Check if start byte is found at start of bytes stream.*/
bool IsStartingWithStartByte(const std::vector<std::byte> bytes_stream_from_imu) {
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
  UpdateContextWithDriveStatusIfChanges(messages::ImuDriverStatus::NO_DATA);
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

void Driver::UpdateContextWithDriveStatusIfChanges(messages::ImuDriverStatus new_status) {
  // if (new_status != internal_driver_status_) {
  driver_context_.SetStatus(new_status);
  internal_driver_status_ = new_status;
  // }
}

void Driver::PollAtAHigherFrequency(std::vector<std::byte>& bytes_stream_from_imu) {
  const auto expected_start_of_new_messages = os_layer_.TimeStampNow();
  while (!driver_must_stop_) {
    bytes_stream_from_imu = ReadBytesFromDevice();
    CleanStreamUpToStartByte(bytes_stream_from_imu);
    if (IsStartingWithStartByte(bytes_stream_from_imu)) {
      break;
    }
    std::this_thread::sleep_for(uart_imu::FRAME_DURATION_US);
    const auto new_timestamp = os_layer_.TimeStampNow();
    const auto duration_since_expected = new_timestamp - expected_start_of_new_messages;
    const auto timeout =
        std::chrono::duration_cast<std::chrono::nanoseconds>(uart_imu::DURATION_BETWEEN_TWO_START_BYTES);
    if (duration_since_expected.count() > timeout.count()) {
      UpdateContextWithDriveStatusIfChanges(messages::ImuDriverStatus::NO_DATA);
    } else {
      // printf("DEBUG duration\n");
      // fflush(stdout);
      // std::cout<< "Old TS: "<<expected_start_of_new_messages.count() << " New TS: "<<new_timestamp.count() <<
      // std::endl; std::cout << "Duration: "<< duration_since_expected.count() <<" Timeout: "<<timeout.count() <<
      // std::endl;
    }
  }
}

void Driver::Run() {
  FlushTheDeviceFile();

  std::vector<std::byte> bytes_stream_from_imu{};

  while (!driver_must_stop_) {
    PollAtAHigherFrequency(bytes_stream_from_imu);
    const auto timestamp = os_layer_.TimeStampNow();
    UpdateContextWithDriveStatusIfChanges(messages::ImuDriverStatus::OK);

    // First byte has been received, now sleep until all the bytes are transferred.
    std::this_thread::sleep_for(uart_imu::MESSAGE_DURATION_US);

    auto byte_read_from_imu = ReadBytesFromDevice();
    bytes_stream_from_imu.insert(bytes_stream_from_imu.end(), byte_read_from_imu.begin(), byte_read_from_imu.end());
    CleanStreamUpToStartByte(bytes_stream_from_imu);
    if (IsThereAtLeastOneMessage(bytes_stream_from_imu)) {
      PushMesagesInQueue(bytes_stream_from_imu, driver_context_, timestamp);
    }
    // Message has been received and pushed. Now sleep until the start of next message.
    std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  }
}

}  // namespace uart_imu
