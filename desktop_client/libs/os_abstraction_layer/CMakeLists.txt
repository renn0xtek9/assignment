SET (LIBNAME os_abstraction_layer)
CREATE_LIBRARY (
  SRCS src/os_abstraction_layer.cpp PUBLIC_HEADERS
  include/${LIBNAME}/os_abstraction_layer.h
  include/${LIBNAME}/os_abstraction_layer_interface.h)

ADD_SUBDIRECTORY (tests)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  ADD_SUBDIRECTORY (benchmark)
ENDIF ()
