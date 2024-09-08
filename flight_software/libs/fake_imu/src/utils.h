// Copyright 2024 <Maxime Haselbauer>
#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <iomanip>
#include <ostream>

inline std::ostream& operator<<(std::ostream& out, const std::byte& byte) {
  out << "0x" << std::hex << std::setfill('0') << std::setw(2) << std::to_integer<int>(byte);
  return out;
}

#endif  // UTILS_H
