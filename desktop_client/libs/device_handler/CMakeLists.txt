SET (LIBNAME device_handler)
CREATE_LIBRARY (
  SRCS
  src/device_handler.cpp
  src/os_abstraction_layer.cpp
  PUBLIC_HEADERS
  include/${LIBNAME}/device_handler.h
  include/${LIBNAME}/os_abstraction_layer.h
  include/${LIBNAME}/os_abstraction_layer_interface.h
  LINK_LIBRARIES
  messages)

ADD_SUBDIRECTORY (tests)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  ADD_SUBDIRECTORY (benchmark)
ENDIF ()
