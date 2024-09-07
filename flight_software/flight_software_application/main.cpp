// Copyright 2024 <Maxime Haselbauer>
#include <flight_software_deployment_linux/flight_software_linux_configuration.h>
#include <os_abstraction_layer/os_abstraction_layer.h>
#include <uart_imu_driver/driver.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "version.hpp"

int main(int, char**) {
  OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{};
  std::mutex imu_message_queue_mutex{};
  std::queue<messages::ImuData> imu_data_queue;

  uart_imu::Driver imu_driver{os_abstraction_layer, imu_data_queue, imu_message_queue_mutex, {kUartImuDeviceFilePath}};

  return 0;
}
