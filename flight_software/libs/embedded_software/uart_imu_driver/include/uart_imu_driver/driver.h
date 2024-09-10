// Copyright 2024 <Maxime Haselbauer>
/*! \file driver.h
 *\brief UART IMU Driver header.
 */
#ifndef UART_IMU_DRIVER_DRIVER_H
#define UART_IMU_DRIVER_DRIVER_H
#include <communication_protocols/uart_imu.h>
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>
#include <os_abstraction_layer/os_abstraction_layer_interface.h>
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

/*! \brief Push new messages in the driver context queue.
 * \param bytes_stream_from_imu vector of bytes from the IMU. This will be empty as messages are pushed.
 * \param driver_context driver context to push the messages to.
 * \param timestamp timestamp of the messages.
 */
void PushMesagesInQueue(std::vector<std::byte>& bytes_stream_from_imu,
                        DriverContext& driver_context,
                        const std::chrono::nanoseconds& timestamp);

/*! \brief Driver class for the IMU
 */
class Driver {
 public:
  /*! \brief Constructor
   * \param os_abastraction_layer Operating system abstraction layer
   * \param driver_context Driver shared memory context
   * \param device_file_path Path to the device file
   */
  explicit Driver(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abastraction_layer,
                  DriverContext& driver_context,
                  const std::string device_file_path);

  /*! \brief Start the driver
   * This will open the device file and start the driver thread if succesfull.
   */
  void Start();

  /*! \brief Stop the driver
   */
  void Stop();

 private:
  /*! \brief Empty the device file by reading all its content and dismissing data.*/
  void FlushTheDeviceFile() const;

  /*! \brief Read bytes from the device file
   * This will read  all bytes available in the device file at once.
   * \return a vector of bytes read from the device file
   */
  std::vector<std::byte> ReadBytesFromDevice() const;

  /*! \brief Poll the device file at regular small interface to catch the start of a message\
   * \param bytes_stream_from_imu vector of bytes from the IMU.
   */
  void PollAtAHigherFrequencyUntilStartByte(std::vector<std::byte>& bytes_stream_from_imu);

  /*! \brief Run function
  Started in separated thread form start.*/
  void Run();

  /*! \brief This will change the internal representation of the driver status.
   * if new_status is different from the current status, the context will be updated.
   * \param new_status
   */
  void UpdateContextWithDriveStatusIfChanges(messages::ImuDriverStatus new_status);

  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_; /**< Operating system abstraction layer */
  DriverContext& driver_context_;                                   /**< Driver shared memory context. */
  const std::string device_file_path_;                              /**< Path to the device file */
  int file_descriptor_{-1};                                         /**< File descriptor handle */
  std::thread driver_thread_;                                       /**< Driver run thread object. */
  std::atomic<bool> driver_must_stop_{false};                       /**< Atomic flag to stop the driver thread. */
  messages::ImuDriverStatus internal_driver_status_{messages::ImuDriverStatus::NO_DATA}; /**< Driver status */
};
}  // namespace uart_imu
#endif  // UART_IMU_DRIVER_DRIVER_H
