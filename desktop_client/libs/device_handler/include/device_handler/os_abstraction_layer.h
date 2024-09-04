// Copyright 2024 <Maxime Haselbauer>
#ifndef DEVICE_HANDLER_OS_ABSTRACTION_LAYER_H
#define DEVICE_HANDLER_OS_ABSTRACTION_LAYER_H
#include <device_handler/os_abstraction_layer_interface.h>
#include <fcntl.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <string>
namespace OsAbstractionLayer {

class OsAbstractionLayer : public OsAbstractionLayerInterface {
 public:
  // virtual ~OsAbstractionLayer() = default;
  /*! \fn void CheckDeviceFileExists
   * \brief This checks whether the device file has been created by the Linux kernel
   * \param device_file_path the path device file to check
   * \return true if the device file exists, false otherwise
   */
  bool CheckDeviceFileExists(const std::string& device_file_path) const final;

  /*! \fn int OpenDeviceFile
   *\brief This Open a device file
   *\param device_file_path the path device file to check
   *\return file_descriptor_id. -1 if the file could not be opened
   */
  int OpenDeviceFile(const std::string& device_file_path) const final;

  /*! \fn int CloseDeviceFile
   *\brief This CloseDeviceFile a device file
   *\param file_descriptor the file descriptor to close
   *\return 0 if the file was closed successfully, -1 otherwise
   */
  int CloseDeviceFile(int file_descriptor) const final;
};

}  // namespace OsAbstractionLayer

#endif  // DEVICE_HANDLER_OS_ABSTRACTION_LAYER_H
