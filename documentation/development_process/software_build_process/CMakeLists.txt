SET (OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/output)
CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/index.html.in
                ${OUTPUT_FOLDER}/index.html)

CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/development_processes.html
                ${OUTPUT_FOLDER}/development_processes.html)

ADD_CUSTOM_COMMAND (
  OUTPUT ${OUTPUT_FOLDER}/graph-packaging.png
  VERBATIM
  COMMAND make -Bnd packaging | make2graph | dot -T png -o
          ${OUTPUT_FOLDER}/graph-packaging.png
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Creating build graph for target: packaging")

ADD_CUSTOM_COMMAND (
  OUTPUT ${OUTPUT_FOLDER}/graph-documentation.png
  VERBATIM
  COMMAND make -Bnd documentation | make2graph | dot -T png -o
          ${OUTPUT_FOLDER}/graph-documentation.png
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Creating build graph for target: documentation")

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
  software_build_process_documentation
  COMMENT "Software build process documentation"
  SOURCES ${CMAKE_SOURCE_DIR}/Makefile
  DEPENDS
    ${OUTPUT_FOLDER}/index.html ${OUTPUT_FOLDER}/build_tree.html
    ${OUTPUT_FOLDER}/graph-packaging.png
    ${OUTPUT_FOLDER}/graph-documentation.png)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  INSTALL (DIRECTORY "${OUTPUT_FOLDER}/"
           DESTINATION "${CMAKE_INSTALL_DOCDIR}/software_build_process")
ENDIF ()
