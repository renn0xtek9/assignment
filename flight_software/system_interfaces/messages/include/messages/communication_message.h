// Copyright 2024 <Maxime Haselbauer>
#ifndef MESSAGES_COMMUNICATION_MESSAGE_H
#define MESSAGES_COMMUNICATION_MESSAGE_H
#include <cstdint>
const uint8_t MAXIMUM_MESSAGE_LENGTH = 64;

struct CommunicationMessage {
  uint8_t message[MAXIMUM_MESSAGE_LENGTH];
  uint16_t message_length;
  int32_t value;
};

#endif  // MESSAGES_COMMUNICATION_MESSAGE_H
