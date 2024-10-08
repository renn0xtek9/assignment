// Copyright 2024 <Maxime Haselbauer>
/*! \file bytes.h
 *\brief ostream overloads for std::bytes
 */
#ifndef DISPLAY_MESSAGES_BYTES_H
#define DISPLAY_MESSAGES_BYTES_H

#include <array>
#include <cstddef>
#include <iomanip>
#include <ostream>
#include <vector>

/*! \brief overload stream operator for std::byte*/
inline std::ostream& operator<<(std::ostream& out, const std::byte& byte) {
  out << "0x" << std::hex << std::setfill('0') << std::setw(2) << std::to_integer<int>(byte);
  return out;
}

/*! \brief overload stream operator for std::vector<std::byte>*/
inline std::ostream& operator<<(std::ostream& out, const std::vector<std::byte>& vec) {
  for (const auto& elem : vec) {
    out << elem << " ";
  }
  return out;
}

/*! \brief overload stream operator for std::array<std::byte,N>*/
template <std::size_t N>
inline std::ostream& operator<<(std::ostream& out, const std::array<std::byte, N>& arr) {
  for (const auto& elem : arr) {
    out << elem << " ";
  }
  return out;
}

#endif
