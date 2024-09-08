// Copyright 2024 <Maxime Haselbauer>
#ifndef OS_ABSTRACTION_LAYER_OS_ABSTRACTION_LAYER_H
#define OS_ABSTRACTION_LAYER_OS_ABSTRACTION_LAYER_H
#include <fcntl.h>
#include <os_abstraction_layer/os_abstraction_layer_interface.h>
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

  /*! \fn std::chrono::nanoseconds TimeStampNow
   * \brief Get the current time in nanoseconds
   * \return current time in nanoseconds
   */
  std::chrono::nanoseconds TimeStampNow() const final;

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

  /*! \fn int Write
   *\brief Write bytes to a file descriptor
   *\param file_descriptor file descriptor integer
   *\param data data to be written
   *\param size size of the data
   *\return number of bytes written
   */
  std::size_t Write(const int& file_descriptor, const void* data, const std::size_t& size) const final;

  /*! \fn int ByteAvailableToRead
  \brief  Check how many bytes are available to read from a file descriptor
  \param file_descriptor file descriptor integer
  \return number of bytes available to read
  */
  int ByteAvailableToRead(const int& file_descriptor) const final;

  /*! \fn void ReadFromFile
   * \param file_descriptor file descriptor integer
   * \param ptr pointer where to write
   * \param size */
  void ReadFromFile(const int& file_descriptor, char* ptr, const std::size_t& size) const final;

  /*! \fn void TruncateFile
   * \param  file_descriptor file descriptor integer
   */
  void TruncateFile(const int& file_descriptor) const final;
};

}  // namespace OsAbstractionLayer

#endif  // DEVICE_HANDLER_OS_ABSTRACTION_LAYER_H
