SET (LIBNAME messages)
ADD_LIBRARY (messages INTERFACE)

TARGET_INCLUDE_DIRECTORIES (
  messages INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
SET_TARGET_PROPERTIES (
  messages PROPERTIES PUBLIC_HEADER include/messages/communication_message.h)
INSTALL (
  TARGETS messages
  PUBLIC_HEADER DESTINATION include/messages/communication_message.h
                COMPONENT Libraries)
