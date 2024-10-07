SET (LIBNAME linux_os_abstraction_layer)
CREATE_LIBRARY (
  SRCS src/linux_os_abstraction_layer.cpp PUBLIC_HEADERS
  include/${LIBNAME}/linux_os_abstraction_layer.h LINK_LIBRARIES
  os_abstraction_layer)
ADD_SUBDIRECTORY (tests)

INSTALL (TARGETS ${LIBNAME} COMPONENT flight_software)
