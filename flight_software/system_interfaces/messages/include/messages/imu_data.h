// Copyright 2024 <Maxime Haselbauer>
#ifndef MESSAGES_IMU_DATA_H
#define MESSAGES_IMU_DATA_H
#include <chrono>
#include <utility>

namespace messages {
struct ImuData {
  float a_x{};
  float a_y{};
  float a_z{};
  float omega_x{};
  float omega_y{};
  float omega_z{};
  float temperature{};
  std::chrono::nanoseconds timestamp{};
};

}  // namespace messages

#endif  // MESSAGES_IMU_DATA_H
