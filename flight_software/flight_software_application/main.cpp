// Copyright 2024 <Maxime Haselbauer>
#include <display_messages/imu_information.h>
#include <flight_software_deployment_linux/flight_software_linux_configuration.h>
#include <os_abstraction_layer/os_abstraction_layer.h>
#include <uart_imu_driver/driver.h>
#include <uart_imu_driver/driver_context.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "version.hpp"

using namespace std::chrono_literals;  // NOLINT

std::atomic<bool> g_stop_required{false};

void signalHandler(int signum) {
  std::cout << std::endl << "Shutdown from user" << std::endl;
  g_stop_required = true;
}

void RetrieveDriverStatus(uart_imu::DriverContext& driver_context, messages::ImuDriverStatus& imu_driver_status) {
  std::lock_guard<std::mutex> lock(driver_context.status_mutex);
  imu_driver_status = driver_context.imu_driver_status;
}

void RetrieveMessage(uart_imu::DriverContext& driver_context,
                     std::queue<messages::ImuData>& imu_message_read_from_driver_queue) {
  std::lock_guard<std::mutex> lock(driver_context.queue_mutex);
  while (!driver_context.imu_data_queue.empty()) {
    imu_message_read_from_driver_queue.push(driver_context.imu_data_queue.front());
    driver_context.imu_data_queue.pop();
  }
}

void DisplayImuInformation(std::queue<messages::ImuData>& imu_message, messages::ImuDriverStatus& imu_driver_status) {
  std::cout << std::endl << "Driver status: " << imu_driver_status << std::endl;
  if (imu_message.empty()) {
    std::cout << "No new IMU data available" << std::endl;
    return;
  } else {
    while (!imu_message.empty()) {
      std::cout << "New IMU data: " << imu_message.front() << std::endl;
      imu_message.pop();
    }
  }
}

int main(int, char**) {
  std::signal(SIGINT, signalHandler);
  std::cout << "Flight software version: " << getVersion() << std::endl;
  OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{};
  uart_imu::DriverContext imu_driver_context{};

  uart_imu::Driver imu_driver{os_abstraction_layer, imu_driver_context, {kUartImuDeviceFilePath}};
  imu_driver.Start();

  std::queue<messages::ImuData> imu_message_read_from_driver_queue{};
  messages::ImuDriverStatus imu_driver_status{};
  while (!g_stop_required) {
    std::this_thread::sleep_for(100ms);
    RetrieveDriverStatus(imu_driver_context, imu_driver_status);
    RetrieveMessage(imu_driver_context, imu_message_read_from_driver_queue);

    DisplayImuInformation(imu_message_read_from_driver_queue, imu_driver_status);
  }
  imu_driver.Stop();

  std::cout << std::endl << "The eagle has landed... (Stopping flight software)" << std::endl;

  return 0;
}
