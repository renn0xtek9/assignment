// Copyright 2024 <Maxime Haselbauer>
#include <device_handler/os_abstraction_layer.h>

#include <cstring>
#include <string>

namespace OsAbstractionLayer {

// LCOV_EXCL_START
int OsAbstractionLayer::CloseDeviceFile(int file_descriptor) const {
  return close(file_descriptor);
}

bool OsAbstractionLayer::CheckDeviceFileExists(const std::string& device_file_path) const {
  if (access(device_file_path.c_str(), F_OK) == 0) {
    return true;
  } else {
    return false;
  }
}

int OsAbstractionLayer::OpenDeviceFile(const std::string& device_file_path) const {
  int file_descriptor = open(device_file_path.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (file_descriptor < 0) {
    std::cerr << "Error opening device file: " << strerror(errno) << std::endl;
    return -1;
  }
  return file_descriptor;
}
// LCOV_EXCL_STOP

}  // namespace OsAbstractionLayer
