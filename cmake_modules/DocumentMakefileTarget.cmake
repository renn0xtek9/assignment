ADD_CUSTOM_COMMAND (
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/output/build_tree.html
  DEPENDS ${CMAKE_BINARY_DIR}/build-documentation/cmake.dot
          ${CMAKE_CURRENT_SOURCE_DIR}/convert_dot_files_to_png.sh
  COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/convert_dot_files_to_png.sh
    ${CMAKE_BINARY_DIR}/build-documentation ${CMAKE_CURRENT_BINARY_DIR}/output
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Creating build graph for configuration: ${CMAKE_BUILD_TYPE}")

ADD_CUSTOM_TARGET (
  software_build_process_documentation ALL
  COMMENT "Software build process documentation"
  SOURCES ${CMAKE_SOURCE_DIR}/Makefile
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/output/index.html
          ${CMAKE_CURRENT_BINARY_DIR}/output/build_tree.html)

SET (DOCUMENTED_MAKEFILE_TARGET "")

INCLUDE (CMakeParseArguments)
# document_makefile_target - A function to document a makefile target
#
# This will create a graph of the dependencies of the target.
#
# DOCUMENT_MAKEFILE_TARGET( [WORKING_DIRECTORY path_of_make_file TARGET
# target_name OUTPUT_DIRECTORY directory where graph must be created] )
#
FUNCTION (document_makefile_target)
  CMAKE_PARSE_ARGUMENTS (
    PARSED_ARGS # Output variable prefix
    "" # No boolean arguments
    "WORKING_DIRECTORY;TARGET;OUTPUT_DIRECTORY" # Mono-valued arguments
    "" # No multi-valued arguments
    ${ARGN} # Arguments to the function
  )

  # Check required arguments
  IF (NOT PARSED_ARGS_WORKING_DIRECTORY)
    MESSAGE (
      FATAL_ERROR "You must provide the working directory of the Makefile.")
  ENDIF ()

  IF (NOT PARSED_ARGS_TARGET)
    MESSAGE (FATAL_ERROR "You must provide the target to document.")
  ENDIF ()

  IF (NOT PARSED_ARGS_OUTPUT_DIRECTORY)
    MESSAGE (
      FATAL_ERROR "You must provide the output directory for the documentation."
    )
  ENDIF ()

  # Define custom command to generate the graph
  ADD_CUSTOM_COMMAND (
    OUTPUT ${PARSED_ARGS_OUTPUT_DIRECTORY}/graph-${PARSED_ARGS_TARGET}.png
    COMMAND make -Bnd ${PARSED_ARGS_TARGET} | make2graph | dot -Tpng -o
            ${PARSED_ARGS_OUTPUT_DIRECTORY}/graph-${PARSED_ARGS_TARGET}.png
    WORKING_DIRECTORY ${PARSED_ARGS_WORKING_DIRECTORY}
    COMMENT "Creating build graph for target: ${PARSED_ARGS_TARGET}"
    VERBATIM)

  # Define a custom target to depend on the graph generation
  ADD_CUSTOM_TARGET (
    ${PARSED_ARGS_TARGET}-documentation ALL
    DEPENDS ${PARSED_ARGS_OUTPUT_DIRECTORY}/graph-${PARSED_ARGS_TARGET}.png
    COMMENT "Building documentation for target: ${PARSED_ARGS_TARGET}")

  # Ensure the documentation target is built before
  # `software_build_process_documentation`
  ADD_DEPENDENCIES (software_build_process_documentation
                    ${PARSED_ARGS_TARGET}-documentation)

  LIST (APPEND DOCUMENTED_MAKEFILE_TARGET ${PARSED_ARGS_TARGET})
  SET (
    DOCUMENTED_MAKEFILE_TARGET
    ${DOCUMENTED_MAKEFILE_TARGET}
    PARENT_SCOPE)

ENDFUNCTION ()
