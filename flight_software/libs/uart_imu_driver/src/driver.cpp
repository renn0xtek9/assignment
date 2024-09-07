// Copyright 2024 <Maxime Haselbauer>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver.h>

#include <thread>
namespace uart_imu {
Driver::Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
               std::queue<messages::ImuData>& imu_data_queue,
               std::mutex& queue_mutex,
               const std::string device_file_path)
    : os_layer_{os_abastrction_layer},
      imu_data_queue_{imu_data_queue},
      queue_mutex_{queue_mutex},
      device_file_path_{device_file_path} {
}

void Driver::Start() {
  printf("IMU dirver is running\n");
  file_descriptor_ = os_layer_.OpenDeviceFile(device_file_path_);
  if (file_descriptor_ < 0) {
    std::cout << "Driver could not open device file: " << device_file_path_ << std::endl;
    Stop();
    return;
  }
}
void Driver::Stop() {
  printf("IMU driver is stopped\n");
}

void Driver::Run() {
  printf("IMU driver step\n");
  std::this_thread::sleep_for(uart_imu::SLEEP_TIME_BETWEEN_MESSAGES_US);
}

}  // namespace uart_imu
