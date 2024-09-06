SET (LIBNAME fake_imu)
CREATE_LIBRARY (
  SRCS
  src/fake_imu.cpp
  PUBLIC_HEADERS
  include/${LIBNAME}/fake_imu.h
  LINK_LIBRARIES
  os_abstraction_layer
  messages)

ADD_SUBDIRECTORY (tests)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  ADD_SUBDIRECTORY (benchmark)
ENDIF ()
