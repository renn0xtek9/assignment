// Copyright 2024 <Maxime Haselbauer>
#ifndef UART_IMU_DRIVER_DRIVER_H
#define UART_IMU_DRIVER_DRIVER_H
#include <messages/imu_data.h>
#include <os_abstraction_layer/os_abstraction_layer.h>

#include <mutex>
#include <queue>
#include <string>
#include <thread>
namespace uart_imu {
class Driver {
 public:
  explicit Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastrction_layer,
                  const std::queue<messages::ImuData>& imu_data_queue,
                  const std::mutex& queue_mutex,
                  const std::string device_file_path);

 private:
  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
  std::queue<messages::ImuData>& imu_data_queue_;
  std::mutex& queue_mutex_;
  const std::string device_file_path_;
};
}  // namespace uart_imu
#endif  // UART_IMU_DRIVER_DRIVER_H
