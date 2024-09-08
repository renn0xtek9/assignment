SET (LIBNAME uart_imu_driver)
CREATE_LIBRARY (
  SRCS
  src/driver.cpp
  PUBLIC_HEADERS
  include/${LIBNAME}/driver.h
  LINK_LIBRARIES
  os_abstraction_layer
  communication_protocols
  serializer
  messages)

ADD_SUBDIRECTORY (tests)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  ADD_SUBDIRECTORY (benchmark)
ENDIF ()
