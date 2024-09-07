// Copyright 2024 <Maxime Haselbauer>
#include <messages/imu_data.h>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver.h>

#include <thread>
namespace uart_imu {
Driver::Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
               DriverContext& driver_context,
               const std::string device_file_path)
    : os_layer_{os_abastrction_layer}, driver_context_{driver_context}, device_file_path_{device_file_path} {
}

void Driver::Start() {
  printf("IMU dirver is running\n");
  file_descriptor_ = os_layer_.OpenDeviceFile(device_file_path_);
  if (file_descriptor_ < 0) {
    std::cout << "Driver could not open device file: " << device_file_path_ << std::endl;
    Stop();
    return;
  }
  driver_thread_ = std::thread(&Driver::Run, this);
}
void Driver::Stop() {
  printf("Stopping IMU driver\n");
  driver_must_stop = true;
  if (driver_thread_.joinable()) {
    driver_thread_.join();
  }
  printf("IMU driver stopped\n");
}

void Driver::Run() {
  printf("IMU driver step\n");
  driver_context_.SetStatus(messages::ImuDriverStatus::OK);
  // TODO(maxime) : truncate device file (we don't want to read old data)
  while (!driver_must_stop) {
    messages::ImuData imu_data{};
    int bytes_available = os_layer_.ByteAvailableToRead(file_descriptor_);
    while (bytes_available < 1) {
      std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
      bytes_available = os_layer_.ByteAvailableToRead(file_descriptor_);
      if (driver_must_stop) {
        return;
      }
    }
    imu_data.timestamp = os_layer_.TimeStampNow();
    std::size_t bytes_to_read = std::min(static_cast<std::size_t>(bytes_available), sizeof(imu_data));
    os_layer_.ReadFromFile(file_descriptor_, reinterpret_cast<char*>(bytes_from_imu.data()), bytes_to_read);
    // std::array<std::byte, bytes_available> payload{};
    // while (not first byte catch)
    // {
    //   // poll for byte available
    // }
    // take the timestamp
    // sleep for duration of message
    // check if enough byte avaialbe
    // read all bytes into byte array
    // convert the message

    std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  }
}

}  // namespace uart_imu
