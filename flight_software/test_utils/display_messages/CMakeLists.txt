SET (LIBNAME display_messages)
SET (LIBHDRS include/${LIBNAME}/imu_information.h)
ADD_LIBRARY (${LIBNAME} INTERFACE ${LIBHDRS})
TARGET_LINK_LIBRARIES (${LIBNAME} INTERFACE messages)
TARGET_INCLUDE_DIRECTORIES (
  ${LIBNAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
SET_TARGET_PROPERTIES (${LIBNAME} PROPERTIES PUBLIC_HEADER "${LIBHDRS}")
INSTALL (TARGETS ${LIBNAME} PUBLIC_HEADER DESTINATION include/${LIBNAME}/
                                          COMPONENT Libraries)
