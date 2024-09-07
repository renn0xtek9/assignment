// Copyright 2024 <Maxime Haselbauer>
#include <flight_software_deployment_linux/flight_software_linux_configuration.h>
#include <os_abstraction_layer/os_abstraction_layer.h>
#include <uart_imu_driver/driver.h>
#include <uart_imu_driver/driver_context.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "version.hpp"

int main(int, char**) {
  OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{};
  uart_imu::DriverContext imu_driver_context{};

  uart_imu::Driver imu_driver{os_abstraction_layer, imu_driver_context, {kUartImuDeviceFilePath}};
  imu_driver.Start();

  return 0;
}
