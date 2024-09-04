SET (OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/output)
CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/index.html.in
                ${OUTPUT_FOLDER}/index.html)

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
  OUTPUT ${OUTPUT_FOLDER}/graph-cmake-${CMAKE_BUILD_TYPE}.png
  DEPENDS ${CMAKE_BINARY_DIR}/cmake.dot
  COMMAND dot -T png -o ${OUTPUT_FOLDER}/graph-cmake-${CMAKE_BUILD_TYPE}.png
          cmake.dot
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Creating build graph for configuration: ${CMAKE_BUILD_TYPE}")

ADD_CUSTOM_TARGET (
  software_build_process_documentation
  COMMENT "Software build process documentation"
  SOURCES ${CMAKE_SOURCE_DIR}/Makefile
  DEPENDS
    ${OUTPUT_FOLDER}/index.html ${OUTPUT_FOLDER}/graph-packaging.png
    ${OUTPUT_FOLDER}/graph-documentation.png
    ${OUTPUT_FOLDER}/graph-cmake-${CMAKE_BUILD_TYPE}.png)

IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  INSTALL (DIRECTORY "${OUTPUT_FOLDER}/"
           DESTINATION "${CMAKE_INSTALL_DOCDIR}/software_build_process")
ENDIF ()
