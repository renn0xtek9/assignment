SET (LIBNAME device_handler)
CREATE_LIBRARY (
  SRCS
  src/device_handler.cpp
  PUBLIC_HEADERS
  include/${LIBNAME}/device_handler.h
  LINK_LIBRARIES
  os_abstraction_layer
  messages)

ADD_SUBDIRECTORY (tests)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  ADD_SUBDIRECTORY (benchmark)
ENDIF ()
