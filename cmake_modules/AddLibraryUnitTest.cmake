INCLUDE (CMakeParseArguments)
# add_library_unit_test - Add a unit test for a library
#
# This function supposes that LIBNAME is set The test will be named
# ${LIBNAME}_test
#
# ADD_LIBRARY_UNIT_TEST( [SRCS <src1> <src2> ...] )
#
# In addition, it also creates a memory_leak_analysis-${LIBNAME} that check for
# memory leaks using valgrind
FUNCTION (add_library_unit_test)

  CMAKE_PARSE_ARGUMENTS (
    PARSED_ARGS # prefix of output variables
    "" # list of names of the boolean arguments (only defined ones will be true)
    "" # list of names of mono-valued arguments
    "SRCS" # list of names of multi-valued arguments (output variables are
    # lists)
    ${ARGN} # arguments of the function to parse, here we take the all original
            # ones
  )
  IF (NOT PARSED_ARGS_SRCS)
    MESSAGE (FATAL_ERROR "You must provide unit test sources for ${LIBNAME}")
  ENDIF (NOT PARSED_ARGS_SRCS)

  FIND_PACKAGE (GTest REQUIRED)
  INCLUDE_DIRECTORIES (${GTEST_INCLUDE_DIRS})
  ADD_EXECUTABLE (${LIBNAME}_test ${PARSED_ARGS_SRCS})

  SET (
    gcc_flags_
    -Werror
    -Wall
    -Wextra
    # -Wpedantic
    -Wshadow
    -Wconversion
    -Wformat=2
    -Wnull-dereference
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wuseless-cast
    # -fanalyzer -fsanitize=address -fsanitize=undefined
    -D_FORTIFY_SOURCE=2)
  TARGET_COMPILE_OPTIONS (${LIBNAME}_test PRIVATE ${gcc_flags_})

  TARGET_LINK_LIBRARIES (
    ${LIBNAME}_test
    ${LIBNAME}
    ${GTEST_MAIN_LIBRARIES}
    ${GTEST_LIBRARIES}
    pthread
    gmock
    gmock_main)
  ADD_TEST (NAME ${LIBNAME}_test COMMAND ${LIBNAME}_test)
  SET_TESTS_PROPERTIES (${LIBNAME}_test PROPERTIES FIXTURES_REQUIRED unit_tests
                                                   LABELS unit_tests)

  ADD_CUSTOM_COMMAND (
    OUTPUT valgrind-${LIBNAME}-out.txt
    COMMAND
      valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
      --verbose --log-file=valgrind-${LIBNAME}-out.txt
      $<TARGET_FILE:${LIBNAME}_test>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Analyze ${LIBNAME}_test with valgrind")

  ADD_CUSTOM_TARGET (
    valgrind-leak-check-${LIBNAME}
    COMMENT "Target: valgrind-leak-check-${LIBNAME}"
    DEPENDS valgrind-${LIBNAME}-out.txt)

  ADD_TEST (
    NAME memory_leak_analysis-${LIBNAME}
    COMMAND make valgrind-leak-check-${LIBNAME}
    CONFIGURATIONS Debug)
  SET_TESTS_PROPERTIES (memory_leak_analysis-${LIBNAME}
                        PROPERTIES FIXTURES_REQUIRED quality LABELS quality)

ENDFUNCTION ()
