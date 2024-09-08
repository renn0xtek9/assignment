// Copyright 2024 <Maxime Haselbauer>
#include <messages/imu_data.h>
#include <serializer/serializer.h>
#include <uart_imu/uart_imu.h>

#include <algorithm>
#include <cstring>
#include <functional>
namespace serializer {
namespace uart {

float UnoffsetTemperature(const float& temperature) {
  return temperature - uart_imu::TEMPERATURE_SENSOR_OFFSET;
}
float OffsetTemperature(const float& temperature) {
  return temperature + uart_imu::TEMPERATURE_SENSOR_OFFSET;
}

std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> Serialize(const messages::ImuData& message) {
  std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> serialized_message{};

  std::size_t index{0U};

  auto emplace_data = [&serialized_message, &index](const float& value, const float& lsb_sensitivity) {
    const auto encoded_value = EncodeFloatOnTwoBytes(value, lsb_sensitivity);
    index = std::min(index,
                     uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION - uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
    const auto start_iteraror = serialized_message.begin() + index;
    std::copy(encoded_value.begin(), encoded_value.end(), start_iteraror);
    index = index + uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING;
  };
  emplace_data(message.a_x, uart_imu::LSB_SENSITIVITY_ACCELERATION);
  emplace_data(message.a_y, uart_imu::LSB_SENSITIVITY_ACCELERATION);
  emplace_data(message.a_z, uart_imu::LSB_SENSITIVITY_ACCELERATION);
  emplace_data(message.omega_x, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY);
  emplace_data(message.omega_y, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY);
  emplace_data(message.omega_z, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY);
  emplace_data(UnoffsetTemperature(message.temperature), uart_imu::LSB_SENSITIVITY_TEMPERATURE);
  return serialized_message;
}

messages::ImuData Deserialize(
    const std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION>& serialized_message) {
  messages::ImuData deserialized_message{};
  std::size_t index{0U};

  auto decode_data = [&serialized_message, &index](float& value, const float& lsb_sensitivity) {
    index = std::min(index,
                     uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION - uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
    std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING> encoded_value{serialized_message[index],
                                                                                      serialized_message[index + 1]};
    value = DecodeFloatFromTwoBytes(encoded_value, lsb_sensitivity);
    index = index + uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING;
  };
  decode_data(deserialized_message.a_x, uart_imu::LSB_SENSITIVITY_ACCELERATION);
  decode_data(deserialized_message.a_y, uart_imu::LSB_SENSITIVITY_ACCELERATION);
  decode_data(deserialized_message.a_z, uart_imu::LSB_SENSITIVITY_ACCELERATION);
  decode_data(deserialized_message.omega_x, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY);
  decode_data(deserialized_message.omega_y, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY);
  decode_data(deserialized_message.omega_z, uart_imu::LSB_SENSITIVITY_ANGULAR_VELOCITY);
  decode_data(deserialized_message.temperature, uart_imu::LSB_SENSITIVITY_TEMPERATURE);
  deserialized_message.temperature = OffsetTemperature(deserialized_message.temperature);
  return deserialized_message;
}

std::array<std::byte, 2> EncodeFloatOnTwoBytes(const float& value, const float& lsb_sensitivity) {
  std::array<std::byte, 2> encoded_value{};
  const float scaled_value_float{value / lsb_sensitivity};
  const std::int16_t scaled_value_integer{static_cast<std::int16_t>(scaled_value_float)};
  std::memcpy(encoded_value.data(), &scaled_value_integer, uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
  return encoded_value;
}

float DecodeFloatFromTwoBytes(const std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING>& encoded_value,
                              const float& lsb_sensitivity) {
  std::int16_t scaled_integer_value{};
  std::memcpy(&scaled_integer_value, encoded_value.data(), uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
  const float scaled_acceleration_float_value{static_cast<float>(scaled_integer_value)};
  return scaled_acceleration_float_value * lsb_sensitivity;
}

}  // namespace uart

}  // namespace serializer
