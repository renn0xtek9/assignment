SET (LIBNAME messages)
SET (LIBHDRS include/messages/driver_status.h include/messages/imu_message.h)
ADD_LIBRARY (messages INTERFACE ${LIBHDRS})

TARGET_INCLUDE_DIRECTORIES (
  messages INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
SET_TARGET_PROPERTIES (messages PROPERTIES PUBLIC_HEADER "${LIBHDRS}")
INSTALL (TARGETS messages PUBLIC_HEADER DESTINATION include/messages/
                                        COMPONENT Libraries)
