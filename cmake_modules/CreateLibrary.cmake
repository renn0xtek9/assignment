INCLUDE (CMakeParseArguments)
# create_library Create a library with the name LIBNAME
#
# This function supposes that LIBNAME is set
#
# CREATE_LIBRARY (SRCS <src1> <src2> ... PUBLIC_HEADERS <header1> <header2>
# ... LINK_LIBRARIES <lib1> <lib2> ...)
#
# And that the folder structure is as follows - CMakeLists.txt - include/ -
# ${LIBNAME}/ - foo.h - bar.h - src/ - private_header.h - foo.c
#
# When compiling in debug, it will be also linked against coverage libraries to
# perform code coverage analysis
FUNCTION (create_library)

  CMAKE_PARSE_ARGUMENTS (
    PARSED_ARGS
    "" # List of boolean names
    "" # list of names of mono-valued arguments
    "SRCS;PUBLIC_HEADERS;LINK_LIBRARIES" # list of names of multi-valued
                                         # arguments (output
    # variables are lists)
    ${ARGN} # arguments of the function to parse, here we take the all original
            # ones
  )
  IF (NOT PARSED_ARGS_SRCS)
    MESSAGE (FATAL_ERROR "No SRCS specified for ${LIBNAME}")
  ENDIF (NOT PARSED_ARGS_SRCS)

  IF (NOT PARSED_ARGS_PUBLIC_HEADERS)
    MESSAGE (WARNING "No PUBLIC_HEADERS specified for ${LIBNAME}")
  ENDIF (NOT PARSED_ARGS_PUBLIC_HEADERS)

  ADD_LIBRARY (${LIBNAME} SHARED ${PARSED_ARGS_SRCS})

  TARGET_INCLUDE_DIRECTORIES (
    ${LIBNAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
                      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

  TARGET_LINK_LIBRARIES (${LIBNAME} PUBLIC ${PARSED_ARGS_LINK_LIBRARIES})

  SET_TARGET_PROPERTIES (${LIBNAME} PROPERTIES PUBLIC_HEADER
                                               "${PARSED_ARGS_PUBLIC_HEADERS}")
  INSTALL (
    TARGETS ${LIBNAME}
    PUBLIC_HEADER DESTINATION include/${LIBNAME}
    LIBRARY DESTINATION lib/ COMPONENT Libraries)

  IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

    SET (gcc_coverage_compile_flags_ -g -O0 -coverage -fprofile-arcs
                                     -ftest-coverage)
    SET (gcc_coverage_link_flags_ -coverage -lgcov -ftest-coverage)

    SET (
      gcc_flags_
      -Werror
      -Wall
      -Wextra
      -Wpedantic
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

    TARGET_COMPILE_OPTIONS (${LIBNAME} PRIVATE ${gcc_coverage_compile_flags_}
                                               ${gcc_flags_})
    TARGET_COMPILE_DEFINITIONS (${LIBNAME} PRIVATE -DDEBUG)

    TARGET_LINK_OPTIONS (${LIBNAME} PRIVATE ${gcc_coverage_link_flags_})
    TARGET_LINK_LIBRARIES (${LIBNAME} PRIVATE --coverage)

  ENDIF ()

ENDFUNCTION ()
