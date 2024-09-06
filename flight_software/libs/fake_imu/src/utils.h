// Copyright 2024 <Maxime Haselbauer>
#ifndef UTILS_H
#define UTILS_H
#include <messages/imu_data.h>

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <ostream>

/*! \brief overload steam operator for ImuData*/
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

inline std::ostream& operator<<(std::ostream& out, const std::byte& byte) {
  out << "0x" << std::hex << std::setfill('0') << std::setw(2) << std::to_integer<int>(byte);
  return out;
}

#endif  // UTILS_H
