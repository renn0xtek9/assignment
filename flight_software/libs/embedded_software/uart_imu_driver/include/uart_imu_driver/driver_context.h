// Copyright 2024 <Maxime Haselbauer>
#ifndef UART_IMU_DRIVER_DRIVER_CONTEXT_H
#define UART_IMU_DRIVER_DRIVER_CONTEXT_H
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>

#include <mutex>
#include <queue>
namespace uart_imu {
struct DriverContext {
  std::queue<messages::ImuData> imu_data_queue{};
  std::mutex queue_mutex{};
  messages::ImuDriverStatus imu_driver_status{messages::ImuDriverStatus::NO_DATA};
  std::mutex status_mutex{};

  void PushData(const messages::ImuData& data) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    imu_data_queue.push(data);
  }

  void SetStatus(messages::ImuDriverStatus status) {
    std::lock_guard<std::mutex> lock(status_mutex);
    imu_driver_status = status;
  }

  messages::ImuDriverStatus GetStatus() {
    std::lock_guard<std::mutex> lock(status_mutex);
    return imu_driver_status;
  }
};
}  // namespace uart_imu
#endif  // UART_IMU_DRIVER_DRIVER_CONTEXT_
