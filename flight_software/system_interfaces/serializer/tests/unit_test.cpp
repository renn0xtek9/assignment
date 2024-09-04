// Copyright 2024 <Maxime Haselbauer>
/*! \file unit_test.cpp
 *\brief unit tests of serializer
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <messages/communication_message.h>
#include <serializer/serializer.h>

#include <cstring>
#include <string>

/*! \test Serialize communication_message to string*/
TEST(CommunicationMessage, SerializeToString) {
  CommunicationMessage message;

  strncpy(reinterpret_cast<char*>(message.message), "Hello\0", MAXIMUM_MESSAGE_LENGTH - 1);

  message.message_length = 5;
  message.value = 42;

  const auto serialized_value = serializer::SerializeToString(message);
  const std::string expected_string{"Hello 42"};

  EXPECT_EQ(expected_string, serialized_value);
}
