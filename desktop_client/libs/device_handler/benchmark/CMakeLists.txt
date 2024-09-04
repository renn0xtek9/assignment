SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
ADD_EXECUTABLE (${LIBNAME}_benchmark ${LIBNAME}_benchmark.cpp)
TARGET_LINK_LIBRARIES (${LIBNAME}_benchmark device_handler)

ADD_TEST (
  NAME ${LIBNAME}_benchmark
  COMMAND $<TARGET_FILE:${LIBNAME}_benchmark> > benchmark_report.txt
  CONFIGURATIONS Release
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
SET_TESTS_PROPERTIES (
  ${LIBNAME}_benchmark PROPERTIES FIXTURES_REQUIRED performance LABELS
                                  performance)
