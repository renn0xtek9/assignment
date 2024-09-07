// Copyright 2024 <Maxime Haselbauer>
#ifndef UART_IMU_DRIVER_DRIVER_H
#define UART_IMU_DRIVER_DRIVER_H
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>
#include <os_abstraction_layer/os_abstraction_layer.h>
#include <uart_imu_driver/driver_context.h>

#include <atomic>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
namespace uart_imu {

class Driver {
 public:
  explicit Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
                  DriverContext& driver_context,
                  const std::string device_file_path);

  void Start();
  void Stop();

 private:
  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
  DriverContext& driver_context_;
  const std::string device_file_path_;
  int file_descriptor_{-1};
  void Run();
  std::thread driver_thread_;
  std::atomic<bool> driver_must_stop{false};
};
}  // namespace uart_imu
#endif  // UART_IMU_DRIVER_DRIVER_H
