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
  COMMAND /usr/bin/objdump -S $<TARGET_FILE:${DESKTOP_CLIENT_APP_NAME}> >
          ${CMAKE_BINARY_DIR}/${DESKTOP_CLIENT_APP_NAME}.dump
  COMMENT "Disassemble ${DESKTOP_CLIENT_APP_NAME}"
  DEPENDS ${DESKTOP_CLIENT_APP_NAME})

ADD_CUSTOM_TARGET (
  convert_to_assembly
  COMMAND g++ -S
  COMMENT "Convert ${DESKTOP_CLIENT_APP_NAME} to assembly"
  DEPENDS ${CMAKE_SOURCE_DIR}/main.cpp)
