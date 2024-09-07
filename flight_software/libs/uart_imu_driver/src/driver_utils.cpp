// Copyright 2024 <Maxime Haselbauer>
#include <uart_imu_driver/driver.h>

#include <cstddef>
#include <vector>
namespace uart_imu {

void CleanEverythingBeforeStartByte(std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES>& byte_array,
                                    std::size_t& last_index_of_valid_data) {
  std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> interediate_byte_array{};
  std::size_t start_byte_index = 0U;
  while ((start_byte_index < last_index_of_valid_data + 1U) && (start_byte_index < sizeof(byte_array))) {
    if (byte_array[start_byte_index] == uart_imu::START_BYTE) {
      break;
    }
    start_byte_index++;
  }
  if (start_byte_index == last_index_of_valid_data + 1U) {
    last_index_of_valid_data = 0U;
    return;
  }
  auto copy_index = start_byte_index;
  while ((copy_index < last_index_of_valid_data + 1U) && (copy_index < sizeof(byte_array))) {
    interediate_byte_array[copy_index - start_byte_index] = byte_array[copy_index];
    copy_index++;
  }
  last_index_of_valid_data = last_index_of_valid_data - start_byte_index + 1U;
  for (std::size_t i = 0U; i < last_index_of_valid_data; i++) {
    byte_array[i] = interediate_byte_array[i];
  }

  //   const auto start_byte_iterator =
  //       std::find(byte_array.begin(), byte_array.begin() + last_index_of_valid_data, uart_imu::START_BYTE);
  //   if (start_byte_iterator == byte_array.begin() + last_index_of_valid_data + 1) {
  //     last_index_of_valid_data = 0U;
  //     return;
  //   }
  //   std::array<std::byte, uart_imu::TOTAL_NUMBER_OF_BYTES> interediate_byte_array{};

  //   std::copy(start_byte_iterator, byte_array.begin() + last_index_of_valid_data + 1,
  //   interediate_byte_array.begin());

  //   last_index_of_valid_data = last_index_of_valid_data - std::distance(byte_array.begin(), start_byte_iterator);

  //   std::copy(interediate_byte_array.begin(), interediate_byte_array.begin() + last_index_of_valid_data + 1,
  //             byte_array.begin());
}

}  // namespace uart_imu
