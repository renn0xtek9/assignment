// Copyright 2024 <Maxime Haselbauer>
#ifndef UART_IMU_DRIVER_DRIVER_H
#define UART_IMU_DRIVER_DRIVER_H
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>
#include <os_abstraction_layer/os_abstraction_layer.h>
#include <uart_imu/uart_imu.h>
#include <uart_imu_driver/driver_context.h>

#include <array>
#include <atomic>
#include <cstddef>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
namespace uart_imu {

class Driver {
 public:
  explicit Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
                  DriverContext& driver_context,
                  const std::string device_file_path);

  void Start();
  void Stop();

 private:
  void FlushTheDeviceFile() const;
  std::vector<std::byte> ReadBytesFromDevice() const;

  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
  DriverContext& driver_context_;
  const std::string device_file_path_;
  int file_descriptor_{-1};
  void Run();
  std::thread driver_thread_;
  std::atomic<bool> driver_must_stop_{false};
};
}  // namespace uart_imu
#endif  // UART_IMU_DRIVER_DRIVER_H
