// Copyright 2024 <Maxime Haselbauer>
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
  while (!driver_must_stop) {
    std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
  }
}

}  // namespace uart_imu
