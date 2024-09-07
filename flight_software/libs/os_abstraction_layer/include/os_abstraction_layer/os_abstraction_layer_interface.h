// Copyright 2024 <Maxime Haselbauer>
#ifndef OS_ABSTRACTION_LAYER_OS_ABSTRACTION_LAYER_INTERFACE_H
#define OS_ABSTRACTION_LAYER_OS_ABSTRACTION_LAYER_INTERFACE_H
#include <fcntl.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <string>

namespace OsAbstractionLayer {

class OsAbstractionLayerInterface {
 public:
  /*! \fn void CheckDeviceFileExists
   * \brief This checks whether the device file has been created by the Linux kernel
   * \param device_file_path the path device file to check
   * \return true if the device file exists, false otherwise
   */
  virtual bool CheckDeviceFileExists(const std::string& device_file_path) const = 0;

  /*! \fn int OpenDeviceFile
   *\brief This Open a device file
   *\param device_file_path the path device file to check
   *\return file_descriptor_id. -1 if the file could not be opened
   */
  virtual int OpenDeviceFile(const std::string& device_file_path) const = 0;

  /*! \fn int CloseDeviceFile
   *\brief This CloseDeviceFile a device file
   *\param file_descriptor the file descriptor to close
   *\return 0 if the file was closed successfully, -1 otherwise
   */
  virtual int CloseDeviceFile(int file_descriptor) const = 0;

  /*! \fn std::size_t Write
  * \brief Write bytes to a file descriptor
  \param file_descriptor file descriptor integer
  \param data data to be written
  \param size size of the data
  \return number of bytes written
  */
  virtual std::size_t Write(const int& file_descriptor, const void* data, const std::size_t& size) const = 0;

  /*! \fn int ByteAvailableToRead
  \brief  Check how many bytes are available to read from a file descriptor
  \param file_descriptor file descriptor integer
  \return number of bytes available to read
  */
  virtual int ByteAvailableToRead(const int& file_descriptor) const = 0;
};

}  // namespace OsAbstractionLayer

#endif  // OS_ABSTRACTION_LAYER_OS_ABSTRACTION_LAYER_INTERFACE_H
