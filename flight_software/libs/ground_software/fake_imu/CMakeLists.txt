SET (LIBNAME fake_imu)
CREATE_LIBRARY (
  SRCS
  src/fake_imu.cpp
  PUBLIC_HEADERS
  include/${LIBNAME}/fake_imu.h
  LINK_LIBRARIES
  os_abstraction_layer
  deployment_configuration
  serializer
  messages
  display_messages)

ADD_SUBDIRECTORY (tests)
