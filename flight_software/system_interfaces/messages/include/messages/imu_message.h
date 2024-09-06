// Copyright 2024 <Maxime Haselbauer>
#ifndef MESSAGES_IMU_MESSAGE_H
#define MESSAGES_IMU_MESSAGE_H
#include <chrono>
#include <utility>

constexpr std::size_t NUMBER_OF_BYTES_FOR_FLOAT_ENCODING{2U};
constexpr std::size_t NUMBER_OF_BYTES_FOR_UART_COMMUNICATION{7U * NUMBER_OF_BYTES_FOR_FLOAT_ENCODING};
constexpr std::pair<float, float> ACCELERATION_RANGE_G{-16.0F, 16.0F};
constexpr std::pair<float, float> ANGULAR_VELOCITY_RANGE_DPS{-250.0F, 250.0F};
constexpr std::pair<float, float> TEMPERATURE_RANGE_C{-40.0F, 85.0F};

constexpr float LSB_SENSITIVITY_ACCELERATION{0.488E-03F};
constexpr float LSB_SENSITIVITY_ANGULAR_VELOCITY{8.75E-3F};
constexpr float LSB_SENSITIVITY_TEMPERATURE{1.0 / 256.0F};
constexpr float TEMPERATURE_SENSOR_OFFSET{25.0F};

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

#endif  // MESSAGES_IMU_MESSAGE_H
