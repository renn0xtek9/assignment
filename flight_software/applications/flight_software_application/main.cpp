// Copyright 2024 <Maxime Haselbauer>
/*! \file main.cpp applications mimic an flightsoftware application that would instantiate the IMU UART driver, read
 * messages from it and display it in the sdtout*/
#include <display_messages/imu_information.h>
#include <flight_software_deployment_linux/flight_software_linux_configuration.h>
#include <os_abstraction_layer/linux_os_abstraction_layer.h>
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

std::atomic<bool> g_stop_required{false}; /**< Flag to stop the software. */

/*! \brief Signal handler for the software
 * This function is called when the user press Ctrl+C in the terminal.
 * \param signum signal number
 */
void signalHandler(int signum) {
  std::cout << std::endl << "Shutdown from user" << std::endl;
  g_stop_required = true;
}

/*! \brief Retrieve the driver status from the driver context
 * \param driver_context driver context
 * \param imu_driver_status driver status to retrieve
 */
void RetrieveDriverStatus(uart_imu::DriverContext& driver_context, messages::ImuDriverStatus& imu_driver_status) {
  std::lock_guard<std::mutex> lock(driver_context.status_mutex);
  imu_driver_status = driver_context.imu_driver_status;
}

/*! \brief Retrieve the new messages from the driver context
 * \param driver_context driver context
 * \param imu_message_read_from_driver_queue queue where the messages are read
 */
void RetrieveMessage(uart_imu::DriverContext& driver_context,
                     std::queue<messages::ImuData>& imu_message_read_from_driver_queue) {
  std::lock_guard<std::mutex> lock(driver_context.queue_mutex);
  while (!driver_context.imu_data_queue.empty()) {
    imu_message_read_from_driver_queue.push(driver_context.imu_data_queue.front());
    driver_context.imu_data_queue.pop();
  }
}

/*! \brief Display all the latest IMU information
 * \param imu_message queue of IMU messages
 * \param imu_driver_status driver status
 */
void DisplayImuInformation(std::queue<messages::ImuData>& imu_message, messages::ImuDriverStatus& imu_driver_status) {
  std::cout << std::endl << "Driver status: " << imu_driver_status << std::endl;
  if (imu_message.empty()) {
    std::cout << "No new IMU data available" << std::endl;
    return;
  } else {
    // Display the last one.
    std::cout << imu_message.size() << " new IMU messages. Last message: " << imu_message.front() << std::endl;
    // Clear the queue
    imu_message = std::queue<messages::ImuData>();
  }
}

/*! entry point.*/
int main(int, char**) {
  std::signal(SIGINT, signalHandler);
  std::cout << "Flight software version: " << getVersion() << std::endl;
  OsAbstractionLayer::LinuxOsAbstractionLayer os_abstraction_layer{};
  uart_imu::DriverContext imu_driver_context{};

  uart_imu::Driver imu_driver{os_abstraction_layer, imu_driver_context, {kUartImuDeviceFilePath}};
  imu_driver.Start();

  std::queue<messages::ImuData> imu_message_read_from_driver_queue{};
  messages::ImuDriverStatus imu_driver_status{};
  while (!g_stop_required) {
    std::this_thread::sleep_for(1s);
    RetrieveDriverStatus(imu_driver_context, imu_driver_status);

    RetrieveMessage(imu_driver_context, imu_message_read_from_driver_queue);

    DisplayImuInformation(imu_message_read_from_driver_queue, imu_driver_status);
  }
  imu_driver.Stop();

  std::cout << std::endl << "The eagle has landed... (Stopping flight software)" << std::endl;

  return 0;
}
