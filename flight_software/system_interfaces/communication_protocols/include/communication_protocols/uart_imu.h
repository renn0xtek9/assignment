// Copyright 2024 <Maxime Haselbauer>
/*! \file uart_imu.h file contains configuration constants for the IMU UART communication protocol.*/
#ifndef COMMUNICATION_PROTOCOLS_UART_IMU_H
#define COMMUNICATION_PROTOCOLS_UART_IMU_H
#include <chrono>
#include <cstddef>
namespace uart_imu {
constexpr int NUMBER_START_BYTE{1};                           /**< Number of start byte at beginning of message.*/
constexpr std::size_t NUMBER_OF_BYTES_FOR_FLOAT_ENCODING{2U}; /**< Number of bytes used by IMU to encode float values.*/
constexpr std::size_t NUMBER_OF_BYTES_FOR_UART_COMMUNICATION{
    7U * NUMBER_OF_BYTES_FOR_FLOAT_ENCODING}; /**< Number of data bytes.*/
constexpr std::size_t TOTAL_NUMBER_OF_BYTES{NUMBER_OF_BYTES_FOR_UART_COMMUNICATION +
                                            NUMBER_START_BYTE}; /**< Number of bytes per messages.*/

constexpr float OUTPUT_DATA_RATE{208.0F}; /**< IMU refresh rate.*/

constexpr float LSB_SENSITIVITY_ACCELERATION{0.488E-03F};   /**< LSB sensitivity of acceleration values.*/
constexpr float LSB_SENSITIVITY_ANGULAR_VELOCITY{8.75E-3F}; /**< LSB sensitivitiy of angular velocities.*/
constexpr float LSB_SENSITIVITY_TEMPERATURE{1.0 / 256.0F};  /**< LSB sensitivity of temperatures.*/
constexpr float TEMPERATURE_SENSOR_OFFSET{25.0F};           /**< Temperature offset.*/

constexpr int BAUDRATE{38400}; /**< Baudrate chosen for UART communication*/
constexpr std::chrono::microseconds BIT_DURATION_US{static_cast<int>(std::chrono::microseconds{1000000}.count()) /
                                                    BAUDRATE}; /**< Transfer duration of a single bit in microseconds.*/
constexpr int NUMBER_OF_BITS_BY_FRAME{10}; /**< Number of bits by frame (1 start bit 8 data bits 1 stop bit.)*/
constexpr int NUMBER_OF_BITS_BY_MESSAGE{NUMBER_OF_BITS_BY_FRAME *
                                        TOTAL_NUMBER_OF_BYTES}; /**< Number of bits for a complete message.*/
constexpr std::chrono::microseconds FRAME_DURATION_US{
    BIT_DURATION_US * NUMBER_OF_BITS_BY_FRAME}; /**< Transfer duration of one frame in microseconds.*/
constexpr std::chrono::microseconds MESSAGE_DURATION_US{
    BIT_DURATION_US * NUMBER_OF_BITS_BY_MESSAGE}; /**< Transfer Duration of one message in microseconds.*/
constexpr std::chrono::microseconds DURATION_BETWEEN_TWO_START_BYTES{
    static_cast<int>(std::chrono::microseconds{1000000}.count()) /
    static_cast<int>(OUTPUT_DATA_RATE)}; /**< Duration between two messages in microseconds.*/

constexpr std::chrono::microseconds DURATION_BETWEEN_END_AND_START_OF_MESSAGE{
    DURATION_BETWEEN_TWO_START_BYTES -
    MESSAGE_DURATION_US}; /**< Idle duration between end and start of messages in microseconds.*/

// Idle a bit less between two message to not miss the next start byte
constexpr std::chrono::microseconds SLEEP_TIME_BETWEEN_MESSAGES_US{
    DURATION_BETWEEN_END_AND_START_OF_MESSAGE -
    FRAME_DURATION_US}; /**< Sleep time of the driver between two messages in microseconds.*/

constexpr std::chrono::microseconds TIMEOUT = DURATION_BETWEEN_TWO_START_BYTES + DURATION_BETWEEN_TWO_START_BYTES;

constexpr std::byte START_BYTE{0xAA}; /**< Value of the start byte.*/

}  // namespace uart_imu

#endif  // COMMUNICATION_PROTOCOLS_UART_IMU_H
