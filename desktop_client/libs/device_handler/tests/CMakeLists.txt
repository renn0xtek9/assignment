ADD_LIBRARY_UNIT_TEST (SRCS unit_test.cpp)

ADD_TEST (
  NAME ${LIBNAME}_test_setup
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND test_setup.sh ${CMAKE_CURRENT_BINARY_DIR})
SET_TESTS_PROPERTIES (${LIBNAME}_test_setup PROPERTIES FIXTURES_SETUP
                                                       "unit_tests;quality")

ADD_TEST (
  NAME ${LIBNAME}_test_cleanup
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND test_cleanup.sh ${CMAKE_CURRENT_BINARY_DIR})
SET_TESTS_PROPERTIES (${LIBNAME}_test_cleanup PROPERTIES FIXTURES_CLEANUP
                                                         "unit_tests;quality")
