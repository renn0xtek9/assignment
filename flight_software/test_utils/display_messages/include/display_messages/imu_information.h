// Copyright 2024 <Maxime Haselbauer>
/*! \file imu_information.h
 *\brief ostream overloads for UART IMU information.
 */
#ifndef DISPLAY_IMU_H
#define DISPLAY_IMU_H
#include <messages/imu_data.h>
#include <messages/imu_driver_status.h>

#include <iomanip>
#include <ostream>

/*! \brief overload stream operator for ImuData*/
inline std::ostream& operator<<(std::ostream& out, const messages::ImuData& data) {
  auto to_time = [&data]() {
    auto epoch = std::chrono::system_clock::time_point(
        std::chrono::duration_cast<std::chrono::system_clock::duration>(data.timestamp));
    std::time_t time = std::chrono::system_clock::to_time_t(epoch);
    std::tm* local_tm = std::localtime(&time);
    return std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
  };

  out << "a_x: " << data.a_x << " a_y: " << data.a_y << " a_z: " << data.a_z << " w_x: " << data.omega_x
      << " w_y: " << data.omega_y << " w_z: " << data.omega_z << " T: " << data.temperature << " time: " << to_time();
  return out;
}

/*! \brief overload stream operator for ImuDriverStatus*/
inline std::ostream& operator<<(std::ostream& out, const messages::ImuDriverStatus& status) {
  switch (status) {
    case messages::ImuDriverStatus::NO_DATA:
      out << "NO_DATA";
      break;
    case messages::ImuDriverStatus::OK:
      out << "OK";
      break;
    case messages::ImuDriverStatus::BUSY:
      out << "BUSY";
      break;
    default:
      out << "UNKNOWN";
      break;
  }
  return out;
}

#endif  // DISPLAY_IMU_H
