CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (stm32-helloworld VERSION 0.0.1)
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
INCLUDE (Packaging)
INCLUDE (AddLibraryUnitTest)
INCLUDE (CreateLibrary)
ENABLE_TESTING ()

SET (CMAKE_SKIP_INSTALL_RPATH TRUE)
SET (INSTALL_RPATH_USE_LINK_PATH TRUE)

ADD_SUBDIRECTORY (desktop_client)
ADD_SUBDIRECTORY (documentation)
