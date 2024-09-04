ADD_TEST (
  NAME code_coverage
  COMMAND gcovr -r .. --fail-under-line 100 --html --html-details -o index.html
  CONFIGURATIONS Debug
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
SET_TESTS_PROPERTIES (
  code_coverage PROPERTIES FIXTURES_REQUIRED "unit_tests;quality" LABELS
                           quality)
SET_TESTS_PROPERTIES (code_coverage PROPERTIES DEPENDS "unit_tests")

ADD_CUSTOM_TARGET (
  disassemble
  COMMAND /usr/bin/objdump -S $<TARGET_FILE:${FLIGHT_SOFTWARE_APP_NAME}> >
          ${CMAKE_BINARY_DIR}/${FLIGHT_SOFTWARE_APP_NAME}.dump
  COMMENT "Disassemble ${FLIGHT_SOFTWARE_APP_NAME}"
  DEPENDS ${FLIGHT_SOFTWARE_APP_NAME})

ADD_CUSTOM_TARGET (
  convert_to_assembly
  COMMAND g++ -S
  COMMENT "Convert ${FLIGHT_SOFTWARE_APP_NAME} to assembly"
  DEPENDS ${CMAKE_SOURCE_DIR}/main.cpp)
