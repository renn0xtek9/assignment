// Copyright 2024 <Maxime Haselbauer>
#ifndef DEVICE_HANDLER_DEVICE_HANDLER_H
#define DEVICE_HANDLER_DEVICE_HANDLER_H
/*! \file device_handler.h
\brief Handle STM32 device
*/
#include <device_handler/os_abstraction_layer.h>
#include <fcntl.h>
#include <messages/communication_message.h>
#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <string>

const char micro_controller_device_file_path[] = "/dev/some_file";

class DeviceHandler {
 public:
  explicit DeviceHandler(const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer);
  ~DeviceHandler() = default;
  DeviceHandler(const DeviceHandler&) = default;
  DeviceHandler(DeviceHandler&&) = default;
  DeviceHandler& operator=(const DeviceHandler&) = default;
  DeviceHandler& operator=(DeviceHandler&&) = default;

  /*! \fn bool HandleDevice
   * \brief This function handles the device
   * \param device_file_path the path device file to check
   * \return This will return false if the handling has to be terminated
   */
  bool HandleDevice(const std::string device_file_path);

 private:
  /*! \fn PublishToDevice
   * \brief This publishes message to the device
   * \param message the message to publish
   */
  void PublishToDevice(const CommunicationMessage& message);
  const OsAbstractionLayer::OsAbstractionLayerInterface& os_layer_;
};

#endif  // DEVICE_HANDLER_DEVICE_HANDLER_H
