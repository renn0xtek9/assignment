SET (LIBNAME os_abstraction_layer)
SET (LIBHDRS include/${LIBNAME}/os_abstraction_layer_interface.h)
ADD_LIBRARY (${LIBNAME} INTERFACE ${LIBHDRS})

TARGET_INCLUDE_DIRECTORIES (
  ${LIBNAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
SET_TARGET_PROPERTIES (${LIBNAME} PROPERTIES PUBLIC_HEADER "${LIBHDRS}")
INSTALL (TARGETS ${LIBNAME} PUBLIC_HEADER DESTINATION include/${LIBNAME}/
                                          COMPONENT Libraries)
