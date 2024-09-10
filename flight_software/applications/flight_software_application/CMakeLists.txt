INCLUDE (GenerateGitHashAtConfigTime)
INCLUDE (GenerateGitHashAtBuildTime)

ADD_EXECUTABLE (${FLIGHT_SOFTWARE_APP_NAME} main.cpp)
TARGET_INCLUDE_DIRECTORIES (${FLIGHT_SOFTWARE_APP_NAME}
                            PRIVATE ${CMAKE_BINARY_DIR})
TARGET_LINK_LIBRARIES (
  ${FLIGHT_SOFTWARE_APP_NAME} PUBLIC uart_imu_driver linux_os_abstraction_layer
                                     deployment_configuration display_messages)

INSTALL (TARGETS ${FLIGHT_SOFTWARE_APP_NAME} RUNTIME DESTINATION bin)
