SET (LIBNAME deployment_configuration)
SET (
  LIBHDRS
  include/software_in_the_loop/sil_configuration.h
  include/flight_software_deployment_linux/flight_software_linux_configuration.h
)

ADD_LIBRARY (${LIBNAME} INTERFACE ${LIBHDRS})

TARGET_INCLUDE_DIRECTORIES (
  ${LIBNAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
SET_TARGET_PROPERTIES (${LIBNAME} PROPERTIES PUBLIC_HEADER "${LIBHDRS}")
INSTALL (TARGETS ${LIBNAME} PUBLIC_HEADER DESTINATION include/${LIBNAME}/
                                          COMPONENT Libraries)
