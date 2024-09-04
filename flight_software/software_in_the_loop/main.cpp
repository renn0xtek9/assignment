// Copyright 2024 <Maxime Haselbauer>
#include <fake_imu/fake_imu.h>
#include <fake_imu/software_in_the_loop_constant.h>
#include <os_abstraction_layer/os_abstraction_layer.h>

#include <chrono>
#include <string>

using namespace std::chrono_literals;  // NOLINT

int main() {
  std::string device_file{kFakeImuSilDeviceFilePath};
  OsAbstractionLayer::OsAbstractionLayer os_abstraction_layer{};
  FakeImu fake_imu{os_abstraction_layer};
  fake_imu.SimulateNormalOperation(device_file, 1000ms);

  return 0;
}
