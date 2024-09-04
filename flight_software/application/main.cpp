// Copyright 2024 <Maxime Haselbauer>
#include <os_abstraction_layer/os_abstraction_layer.h>

#include <iostream>
#include <memory>
#include <vector>

#include "version.hpp"

int main(int, char**) {
  OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{};

  const std::string serial_over_usb_device_file_path{"/dev/ttyACM0"};

  return 0;
}
