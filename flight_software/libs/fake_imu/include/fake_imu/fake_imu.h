// Copyright 2024 <Maxime Haselbauer>
#ifndef FAKE_IMU_FAKE_IMU_H
#define FAKE_IMU_FAKE_IMU_H
#include <messages/imu_data.h>
#include <os_abstraction_layer/os_abstraction_layer_interface.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <queue>
#include <string>

/*! \brief Fake an IMU device for usage in Software in the loop context
 */
class FakeImu {
 public:
  /*! \ brief Instatiate fake IMU device
   *\param os_abstraction_layer reference to an operating system abstraction layer.
   */
  explicit FakeImu(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abstraction_layer);

  /*! \brief Simulate normal operation of the IMU
   *This sends data in the virtual serial device specified by the file descriptor for a given number of seconds
   *\param device_file_path the path to the device file
   *\param duration_ms duration for which data shalll be sent
   */
  void SimulateNormalOperation(const std::string& device_file_path, const std::chrono::milliseconds& duration_ms);

 private:
  /*! \brief Send a byte to the device file
   */
  void SendByte(const std::byte& byte);

  /*! \brief Send IMU data to the device file
   */
  void SendImuData(const messages::ImuData& data);

  int file_descriptor_{-1};

  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
};

#endif  // FAKE_IMU_FAKE_IMU_H
