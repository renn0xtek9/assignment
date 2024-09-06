// Copyright 2024 <Maxime Haselbauer>
#ifndef MESSAGES_IMU_DATA_H
#define MESSAGES_IMU_DATA_H
#include <chrono>
#include <utility>

constexpr std::size_t NUMBER_OF_BYTES_FOR_FLOAT_ENCODING{2U};
constexpr std::size_t NUMBER_OF_BYTES_FOR_UART_COMMUNICATION{7U * NUMBER_OF_BYTES_FOR_FLOAT_ENCODING};

constexpr float LSB_SENSITIVITY_ACCELERATION{0.488E-03F};
constexpr float LSB_SENSITIVITY_ANGULAR_VELOCITY{8.75E-3F};
constexpr float LSB_SENSITIVITY_TEMPERATURE{1.0 / 256.0F};
constexpr float TEMPERATURE_SENSOR_OFFSET{25.0F};

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
