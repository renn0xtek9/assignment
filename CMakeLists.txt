CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
FILE (COPY cmake_modules/CMakeGraphVizOptions.cmake
      DESTINATION ${CMAKE_BINARY_DIR})
SET (GLOBAL_VERSION 0.0.1)
PROJECT (flight-software VERSION ${GLOBAL_VERSION})
ENABLE_TESTING ()
FIND_PROGRAM (CONAN conan)
EXECUTE_PROCESS (
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND
    ${CONAN} install ../conanfile.txt
    --profile:build=../conan_profiles/linux-x86
    --profile:host=../conan_profiles/linux-x86 --build missing
    COMMAND_ERROR_IS_FATAL ANY)

LIST (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/)

INCLUDE (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP (PROFILE ${CMAKE_SOURCE_DIR}/conan_profiles/linux-x86)

INCLUDE (GNUInstallDirs)
INCLUDE (AddLibraryUnitTest)
INCLUDE (CreateLibrary)
ENABLE_TESTING ()

SET (CMAKE_SKIP_INSTALL_RPATH TRUE)
SET (INSTALL_RPATH_USE_LINK_PATH TRUE)

ADD_SUBDIRECTORY (flight_software)
# ADD_SUBDIRECTORY (embedded_software)
ADD_SUBDIRECTORY (documentation)

ADD_SUBDIRECTORY (packaging)
