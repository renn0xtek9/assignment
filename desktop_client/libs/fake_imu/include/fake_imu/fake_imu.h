// Copyright 2024 <Maxime Haselbauer>
#ifndef FAKE_IMU_FAKE_IMU_H
#define FAKE_IMU_FAKE_IMU_H

#include <os_abstraction_layer/os_abstraction_layer_interface.h>

#include <string>

/*! \brief Fake an IMU device for usage in Software in the loop context
 */
class FakeImu {
 public:
  /*! \ brief Instatiate fake IMU device
  \param os_abstraction_layer reference to an operating system abstraction layer.
  */
  explicit FakeImu(const OsAbstractionLayer::OsAbstractionLayerInterface& os_abstraction_layer);

  /*! \brief Simulate normal operation of the IMU
  This sends data in the virtual serial device specified by the file descriptor
    \param device_file_path the path to the device file
  */
  void SimulateNormalOperation(const std::string& device_file_path);

 private:
  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
};

#endif  // FAKE_IMU_FAKE_IMU_H
