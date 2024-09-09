// Copyright 2024 <Maxime Haselbauer>
#ifndef SERIALIZER_SERIALIZER_H
#define SERIALIZER_SERIALIZER_H
#include <communication_protocols/uart_imu.h>
#include <messages/imu_data.h>

#include <array>
#include <cstddef>
namespace serializer {
namespace uart {

/*! \brief Serialize an ImuData message to an array of bytes
 *  \param message The ImuData message to serialize
 *  \return The serialized message as an array of bytes
 */
std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> Serialize(const messages::ImuData& message);

/*! \brief Deserialize an imu message
 *  \param serialized_message The serialized message to deserialize
 *  \return The deserialized ImuData message
 */
messages::ImuData Deserialize(
    const std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_UART_COMMUNICATION>& serialized_message);

/*! \brief Encode a float value on two bytes
 *   \param value the value to be encoded
 *   \param lsb_sensitivity the least significant bit sensitivity of the sensor for this particular data
 *   \return array of two bytes representing the float value. The low byte is in the first position.
 */
std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING> EncodeFloatOnTwoBytes(const float& value,
                                                                                          const float& lsb_sensitivity);

/*! \brief Decode an array of two bytes into a float value
 *
 *  \param encoded_acceleration The array of two bytes to decode. The low byte is in the first position.
 *  \param lsb_sensitivity The least significant bit sensitivity of the sensor for this particular data
 *  \return The decoded float value
 */
float DecodeFloatFromTwoBytes(
    const std::array<std::byte, uart_imu::NUMBER_OF_BYTES_FOR_FLOAT_ENCODING>& encoded_byte_array,
    const float& lsb_sensitivity);

}  // namespace uart
}  // namespace serializer

#endif  // SERIALIZER_SERIALIZER_H
