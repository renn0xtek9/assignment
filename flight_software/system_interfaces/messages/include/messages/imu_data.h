// Copyright 2024 <Maxime Haselbauer>
#ifndef MESSAGES_IMU_DATA_H
#define MESSAGES_IMU_DATA_H
#include <chrono>
#include <utility>

namespace messages {

/*! \brief IMU messages produced by the IMU UART driver.*/
struct ImuData {
  float a_x{};                          /**< Acceleration along x (g) */
  float a_y{};                          /**< Acceleration along y (g) */
  float a_z{};                          /**< Acceleration along z (g) */
  float omega_x{};                      /**< Rotation rate along x (dps) */
  float omega_y{};                      /**< Rotation rate along y (dps) */
  float omega_z{};                      /**< Rotation rate along z (dps) */
  float temperature{};                  /**< Temperature (deg.C) */
  std::chrono::nanoseconds timestamp{}; /**< Timestamp since epoch (ns) */
};

}  // namespace messages

#endif  // MESSAGES_IMU_DATA_H
