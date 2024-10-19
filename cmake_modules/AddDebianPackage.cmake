INCLUDE (CMakeParseArguments)
INCLUDE (InstallRequiredSystemLibraries)

ADD_CUSTOM_TARGET (package COMMENT "Creating all packages artifacts")

# add_debian_package - Add a debian package
#
# It will use the file ${TARGET_PACKAGE_NAME}.cmake.in to collect information on
# how to create the package It will use the
# ${TARGET_PACKAGE_NAME}-description.txt file to provide the package description
# to create the package. The target will be added to the dependencies of the
# "package" target.
#
# ADD_DEBIAN_PACKAGE( TARGET_PACKAGE_NAME <package-name> )
#
FUNCTION (add_debian_package)
  CMAKE_PARSE_ARGUMENTS (
    PARSED_ARGS # Output variable prefix
    "" # No boolean arguments
    "TARGET_PACKAGE_NAME" # Mono-valued arguments
    "" # No multi-valued arguments
    ${ARGN} # Arguments to the function
  )

  # Check required arguments
  IF (NOT PARSED_ARGS_TARGET_PACKAGE_NAME)
    MESSAGE (FATAL_ERROR "You must provide the target package name.")
  ENDIF ()

  SET (cpack_package_name_ "${PARSED_ARGS_TARGET_PACKAGE_NAME}")
  STRING (
    CONCAT
      CPACK_PACKAGE_FILE_NAME
      "${PROJECT_NAME}-"
      "${cpack_package_name_}-"
      "${PROJECT_VERSION_MAJOR}-"
      "${PROJECT_VERSION_MINOR}-"
      "${PROJECT_VERSION_PATCH}-"
      "Linux")
  CONFIGURE_FILE (
    ${cpack_package_name_}.cmake.in
    ${CMAKE_BINARY_DIR}/Packaging/CPackConfig-${cpack_package_name_}.cmake
    @ONLY)
  ADD_CUSTOM_TARGET (
    package_${cpack_package_name_}
    COMMAND
      cpack -G DEB --config
      ${CMAKE_BINARY_DIR}/Packaging/CPackConfig-${cpack_package_name_}.cmake
    BYPRODUCTS ${CPACK_PACKAGE_FILE_NAME}.deb
    COMMENT "Packaging ${PROJECT_NAME} - ${cpack_package_name_}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS
      ${CMAKE_BINARY_DIR}/Packaging/CPackConfig-${cpack_package_name_}.cmake
    VERBATIM)
  ADD_DEPENDENCIES (package package_${cpack_package_name_})
ENDFUNCTION ()
