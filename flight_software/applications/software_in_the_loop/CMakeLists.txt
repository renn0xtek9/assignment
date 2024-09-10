SET (SOFTWARE_IN_LOOP_APP_NAME software_in_the_loop)

ADD_EXECUTABLE (${SOFTWARE_IN_LOOP_APP_NAME} main.cpp)
TARGET_INCLUDE_DIRECTORIES (${SOFTWARE_IN_LOOP_APP_NAME}
                            PRIVATE ${CMAKE_BINARY_DIR})
TARGET_LINK_LIBRARIES (
  ${SOFTWARE_IN_LOOP_APP_NAME} PUBLIC fake_imu linux_os_abstraction_layer
                                      deployment_configuration)

INSTALL (TARGETS ${SOFTWARE_IN_LOOP_APP_NAME} RUNTIME DESTINATION bin)
