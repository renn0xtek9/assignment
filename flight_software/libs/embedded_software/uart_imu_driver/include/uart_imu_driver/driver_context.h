// Copyright 2024 <Maxime Haselbauer>
#ifndef UART_IMU_DRIVER_DRIVER_CONTEXT_H
#define UART_IMU_DRIVER_DRIVER_CONTEXT_H
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>

#include <mutex>
#include <queue>
namespace uart_imu {

/*! \brief Shared memory context for the dirver
 * \note in an actual embedded software, this shall need to be implemented. Instead, the middleware will provide some
 * publisher/subscriber mechanism. */
struct DriverContext {
  std::queue<messages::ImuData> imu_data_queue{}; /**< Message queue to where data are pushed. */
  std::mutex queue_mutex{};                       /**< Mutex for locking access to message queue. */
  messages::ImuDriverStatus imu_driver_status{messages::ImuDriverStatus::NO_DATA}; /**< Driver status. */
  std::mutex status_mutex{}; /**< Mutex for locking access to driver status. */

  /*! \brief Push data in the queue
   * \param data data to push in the queue
   */
  void PushData(const messages::ImuData& data) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    imu_data_queue.push(data);
  }

  /*! \brief Set the driver status
   * \param status new status to set
   */
  void SetStatus(messages::ImuDriverStatus status) {
    std::lock_guard<std::mutex> lock(status_mutex);
    imu_driver_status = status;
  }

  /*! \brief Get the driver status
   * \return the driver status
   */
  messages::ImuDriverStatus GetStatus() {
    std::lock_guard<std::mutex> lock(status_mutex);
    return imu_driver_status;
  }
};
}  // namespace uart_imu
#endif  // UART_IMU_DRIVER_DRIVER_CONTEXT_
