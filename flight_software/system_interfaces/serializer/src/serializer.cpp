// Copyright 2024 <Maxime Haselbauer>
#include <messages/imu_message.h>
#include <serializer/serializer.h>

#include <cstring>
#include <functional>
namespace serializer {
namespace uart {

float UnoffsetTemperature(const float& temperature) {
  return temperature - TEMPERATURE_SENSOR_OFFSET;
}
float OffsetTemperature(const float& temperature) {
  return temperature + TEMPERATURE_SENSOR_OFFSET;
}

std::array<std::byte, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> Serialize(const ImuData& message) {
  std::array<std::byte, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> serialized_message{};

  std::size_t index{0U};

  auto emplace_data = [&serialized_message, &index](const float& value, const float& lsb_sensitivity) {
    const auto encoded_value = EncodeFloatOnTwoBytes(value, lsb_sensitivity);
    index = std::min(index, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION - NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
    std::memcpy(serialized_message.data() + index, encoded_value.data(), encoded_value.size());
    index = index + NUMBER_OF_BYTES_FOR_FLOAT_ENCODING;
  };
  emplace_data(message.a_x, LSB_SENSITIVITY_ACCELERATION);
  emplace_data(message.a_y, LSB_SENSITIVITY_ACCELERATION);
  emplace_data(message.a_z, LSB_SENSITIVITY_ACCELERATION);
  emplace_data(message.omega_x, LSB_SENSITIVITY_ANGULAR_VELOCITY);
  emplace_data(message.omega_y, LSB_SENSITIVITY_ANGULAR_VELOCITY);
  emplace_data(message.omega_z, LSB_SENSITIVITY_ANGULAR_VELOCITY);
  emplace_data(UnoffsetTemperature(message.temperature), LSB_SENSITIVITY_TEMPERATURE);
  return serialized_message;
}

ImuData Deserialize(const std::array<std::byte, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION>& serialized_message) {
  ImuData deserialized_message{};
  std::size_t index{0U};

  auto decode_data = [&serialized_message, &index](float& value, const float& lsb_sensitivity) {
    index = std::min(index, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION - NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
    std::array<std::byte, NUMBER_OF_BYTES_FOR_FLOAT_ENCODING> encoded_value{serialized_message[index],
                                                                            serialized_message[index + 1]};
    value = DecodeFloatFromTwoBytes(encoded_value, lsb_sensitivity);
    index = index + NUMBER_OF_BYTES_FOR_FLOAT_ENCODING;
  };
  decode_data(deserialized_message.a_x, LSB_SENSITIVITY_ACCELERATION);
  decode_data(deserialized_message.a_y, LSB_SENSITIVITY_ACCELERATION);
  decode_data(deserialized_message.a_z, LSB_SENSITIVITY_ACCELERATION);
  decode_data(deserialized_message.omega_x, LSB_SENSITIVITY_ANGULAR_VELOCITY);
  decode_data(deserialized_message.omega_y, LSB_SENSITIVITY_ANGULAR_VELOCITY);
  decode_data(deserialized_message.omega_z, LSB_SENSITIVITY_ANGULAR_VELOCITY);
  decode_data(deserialized_message.temperature, LSB_SENSITIVITY_TEMPERATURE);
  deserialized_message.temperature = OffsetTemperature(deserialized_message.temperature);
  return deserialized_message;
}

std::array<std::byte, 2> EncodeFloatOnTwoBytes(const float& value, const float& lsb_sensitivity) {
  std::array<std::byte, 2> encoded_value{};
  const float scaled_value_float{value / lsb_sensitivity};
  const std::int16_t scaled_value_integer{static_cast<std::int16_t>(scaled_value_float)};
  std::memcpy(encoded_value.data(), &scaled_value_integer, NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
  return encoded_value;
}

float DecodeFloatFromTwoBytes(const std::array<std::byte, NUMBER_OF_BYTES_FOR_FLOAT_ENCODING>& encoded_value,
                              const float& lsb_sensitivity) {
  std::int16_t scaled_integer_value{};
  std::memcpy(&scaled_integer_value, encoded_value.data(), NUMBER_OF_BYTES_FOR_FLOAT_ENCODING);
  const float scaled_acceleration_float_value{static_cast<float>(scaled_integer_value)};
  return scaled_acceleration_float_value * lsb_sensitivity;
}

}  // namespace uart

}  // namespace serializer
