// Copyright 2024 <Maxime Haselbauer>
/*! \file unit_test.cpp
 *\brief unit tests of serializer
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <messages/imu_data.h>
#include <serializer/serializer.h>

#include <cstring>
#include <string>

/*! \test Deserialize imu float value from UART*/
TEST(DeserializeUartData, DecodeFloatFromTwoBytes) {
  // An encoded acceleration with value of 2 times the lsb sensitivity with the high byte in second position.
  std::array<std::byte, 2> encoded_acceleration{std::byte{0x02}, std::byte{0x00}};

  const float decoded_acceleration =
      serializer::uart::DecodeFloatFromTwoBytes(encoded_acceleration, LSB_SENSITIVITY_ACCELERATION);
  EXPECT_NEAR(2.0F * LSB_SENSITIVITY_ACCELERATION, decoded_acceleration, LSB_SENSITIVITY_ACCELERATION);
}

/*! \test Serialize imu float value to UART*/
TEST(SerializeUartData, EncodeFloatOnTwoBytes) {
  const std::array<std::byte, 2> encoded_acceleration =
      serializer::uart::EncodeFloatOnTwoBytes(LSB_SENSITIVITY_ACCELERATION, LSB_SENSITIVITY_ACCELERATION);

  std::array<std::byte, 2> expected_encoded_acceleration{std::byte{0x01}, std::byte{0x00}};
  EXPECT_EQ(expected_encoded_acceleration, encoded_acceleration);
}

/*! \test Serialize IMU data to UART*/
TEST(SerializeUartData, Serialize) {
  const auto two_lsb_acc{2.0F * LSB_SENSITIVITY_ACCELERATION};
  const auto two_lsb_ang{2.0F * LSB_SENSITIVITY_ANGULAR_VELOCITY};

  messages::ImuData imu_data{two_lsb_acc,
                             two_lsb_acc,
                             two_lsb_acc,
                             two_lsb_ang,
                             two_lsb_ang,
                             two_lsb_ang,
                             TEMPERATURE_SENSOR_OFFSET,
                             std::chrono::nanoseconds{0}};

  const std::array<std::byte, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> serialized_data =
      serializer::uart::Serialize(imu_data);

  std::array<std::byte, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> expected_serialized_data{
      std::byte{0x02}, std::byte{0x00}, std::byte{0x02}, std::byte{0x00}, std::byte{0x02},
      std::byte{0x00}, std::byte{0x02}, std::byte{0x00}, std::byte{0x02}, std::byte{0x00},
      std::byte{0x02}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00}};
  EXPECT_EQ(expected_serialized_data, serialized_data);
}

/*! \test Deserialize IMU data from UART*/
TEST(DeserializeUartData, Deserialize) {
  std::array<std::byte, NUMBER_OF_BYTES_FOR_UART_COMMUNICATION> serialized_data{
      std::byte{0x02}, std::byte{0x00}, std::byte{0x02}, std::byte{0x00}, std::byte{0x02},
      std::byte{0x00}, std::byte{0x02}, std::byte{0x00}, std::byte{0x02}, std::byte{0x00},
      std::byte{0x02}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00}};
  const messages::ImuData expected_imu_data{2.0F * LSB_SENSITIVITY_ACCELERATION,
                                            2.0F * LSB_SENSITIVITY_ACCELERATION,
                                            2.0F * LSB_SENSITIVITY_ACCELERATION,
                                            2.0F * LSB_SENSITIVITY_ANGULAR_VELOCITY,
                                            2.0F * LSB_SENSITIVITY_ANGULAR_VELOCITY,
                                            2.0F * LSB_SENSITIVITY_ANGULAR_VELOCITY,
                                            TEMPERATURE_SENSOR_OFFSET,
                                            std::chrono::nanoseconds{0}};

  EXPECT_NEAR(expected_imu_data.a_x, serializer::uart::Deserialize(serialized_data).a_x, LSB_SENSITIVITY_ACCELERATION);
  EXPECT_NEAR(expected_imu_data.a_y, serializer::uart::Deserialize(serialized_data).a_y, LSB_SENSITIVITY_ACCELERATION);
  EXPECT_NEAR(expected_imu_data.a_z, serializer::uart::Deserialize(serialized_data).a_z, LSB_SENSITIVITY_ACCELERATION);
  EXPECT_NEAR(expected_imu_data.omega_x, serializer::uart::Deserialize(serialized_data).omega_x,
              LSB_SENSITIVITY_ANGULAR_VELOCITY);
  EXPECT_NEAR(expected_imu_data.omega_y, serializer::uart::Deserialize(serialized_data).omega_y,
              LSB_SENSITIVITY_ANGULAR_VELOCITY);
  EXPECT_NEAR(expected_imu_data.omega_z, serializer::uart::Deserialize(serialized_data).omega_z,
              LSB_SENSITIVITY_ANGULAR_VELOCITY);
  EXPECT_NEAR(expected_imu_data.temperature, serializer::uart::Deserialize(serialized_data).temperature,
              LSB_SENSITIVITY_TEMPERATURE);
  EXPECT_EQ(expected_imu_data.timestamp, serializer::uart::Deserialize(serialized_data).timestamp);
}
