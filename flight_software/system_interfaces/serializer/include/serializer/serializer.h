// Copyright 2024 <Maxime Haselbauer>
#ifndef SERIALIZER_SERIALIZER_H
#define SERIALIZER_SERIALIZER_H
#include <messages/communication_message.h>

#include <string>
namespace serializer {

std::string SerializeToString(const CommunicationMessage& message);

}

#endif  // SERIALIZER_SERIALIZER_H
