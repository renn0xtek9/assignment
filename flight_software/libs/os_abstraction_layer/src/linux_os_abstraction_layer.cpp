// Copyright 2024 <Maxime Haselbauer>
#include <os_abstraction_layer/linux_os_abstraction_layer.h>
#include <sys/ioctl.h>

#include <cstring>
#include <string>

namespace OsAbstractionLayer {

// LCOV_EXCL_START
int LinuxOsAbstractionLayer::CloseDeviceFile(int file_descriptor) const {
  return close(file_descriptor);
}

std::chrono::nanoseconds LinuxOsAbstractionLayer::TimeStampNow() const {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
}

int LinuxOsAbstractionLayer::OpenDeviceFile(const std::string& device_file_path) const {
  int file_descriptor = open(device_file_path.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (file_descriptor < 0) {
    std::cerr << "Error opening device file: " << strerror(errno) << std::endl;
    return -1;
  }
  return file_descriptor;
}

std::size_t LinuxOsAbstractionLayer::Write(const int& file_descriptor,
                                           const void* data,
                                           const std::size_t& size) const {
  return write(file_descriptor, data, static_cast<size_t>(size));
}

int LinuxOsAbstractionLayer::ByteAvailableToRead(const int& file_descriptor) const {
  int bytesAvailable{};
  if (ioctl(file_descriptor, FIONREAD, &bytesAvailable) == -1) {
    std::cerr << "Failed to get available bytes: " << std::strerror(errno) << std::endl;
    close(file_descriptor);
    return -1;
  }
  return bytesAvailable;
}

void LinuxOsAbstractionLayer::ReadFromFile(const int& file_descriptor, char* ptr, const std::size_t& size) const {
  const ssize_t bytes_read = read(file_descriptor, ptr, size);
  if (bytes_read > 0) {
    return;
  } else if (bytes_read == -1) {
    std::cerr << "Failed to read from file descriptor: " << std::strerror(errno) << std::endl;
  } else {
    std::cerr << "End of file reached or no data available" << std::endl;
  }
}

// LCOV_EXCL_STOP

}  // namespace OsAbstractionLayer
