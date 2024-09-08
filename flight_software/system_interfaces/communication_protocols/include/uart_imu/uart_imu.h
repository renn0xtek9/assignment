// Copyright 2024 <Maxime Haselbauer>
#ifndef UART_IMU_UART_IMU_H
#define UART_IMU_UART_IMU_H
#include <chrono>
#include <cstddef>
namespace uart_imu {
constexpr int NUMBER_START_BYTE{1};
constexpr std::size_t NUMBER_OF_BYTES_FOR_FLOAT_ENCODING{2U};
constexpr std::size_t NUMBER_OF_BYTES_FOR_UART_COMMUNICATION{7U * NUMBER_OF_BYTES_FOR_FLOAT_ENCODING};
constexpr std::size_t TOTAL_NUMBER_OF_BYTES{NUMBER_OF_BYTES_FOR_UART_COMMUNICATION + NUMBER_START_BYTE};

constexpr float OUTPUT_DATA_RATE{208.0F};

constexpr float LSB_SENSITIVITY_ACCELERATION{0.488E-03F};
constexpr float LSB_SENSITIVITY_ANGULAR_VELOCITY{8.75E-3F};
constexpr float LSB_SENSITIVITY_TEMPERATURE{1.0 / 256.0F};
constexpr float TEMPERATURE_SENSOR_OFFSET{25.0F};

constexpr int BAUDRATE{38400};
constexpr std::chrono::microseconds BIT_DURATION_US{static_cast<int>(std::chrono::microseconds{1000000}.count()) /
                                                    BAUDRATE};
constexpr int NUMBER_OF_BITS_BY_FRAME{10};  // 1 start bit + 8 data bits + 1 stop bit
constexpr int NUMBER_OF_BITS_BY_MESSAGE{NUMBER_OF_BITS_BY_FRAME * TOTAL_NUMBER_OF_BYTES};
constexpr std::chrono::microseconds FRAME_DURATION_US{BIT_DURATION_US * NUMBER_OF_BITS_BY_FRAME};
constexpr std::chrono::microseconds MESSAGE_DURATION_US{BIT_DURATION_US * NUMBER_OF_BITS_BY_MESSAGE};

constexpr std::chrono::microseconds DURATION_BETWEEN_TWO_START_BYTES{
    static_cast<int>(std::chrono::microseconds{1000000}.count()) / static_cast<int>(OUTPUT_DATA_RATE)};
constexpr std::chrono::microseconds SLEEP_TIME_BETWEEN_MESSAGES_US{DURATION_BETWEEN_TWO_START_BYTES -
                                                                   MESSAGE_DURATION_US};

constexpr std::byte START_BYTE{0xAA};

}  // namespace uart_imu

#endif  // UART_IMU_UART_IMU_H
