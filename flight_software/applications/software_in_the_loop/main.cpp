// Copyright 2024 <Maxime Haselbauer>
/*! \file This application mimic a "software in the loop" i.e. it sends fake IMU data over a virtual serial device.*/
#include <fake_imu/fake_imu.h>
#include <os_abstraction_layer/linux_os_abstraction_layer.h>
#include <software_in_the_loop/software_in_the_loop_configuration.h>

#include <chrono>
#include <string>

#include "parameters.h"

using namespace std::chrono_literals;  // NOLINT

int main(int argc, char* argv[]) {
  SoftwareInTheLoopRunParameters run_parameters = ParseArguments(argc, argv);

  std::string device_file{kFakeImuSilDeviceFilePath};
  OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{};
  FakeImu fake_imu{os_abstraction_layer};
  fake_imu.SimulateNormalOperation(device_file, run_parameters.duration_ms);

  return 0;
}
