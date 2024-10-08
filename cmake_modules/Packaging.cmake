INCLUDE (InstallRequiredSystemLibraries)
SET (CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET (CPACK_PACKAGE_VENDOR "Maxime Haselbauer")
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - Asignement work")
SET (CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
SET (CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/renn0xtek9/assignment")
SET (CPACK_PACKAGE_CONTACT "maxime.haselbauer@googlemail.com")
SET (CPACK_PACKAGE_VERSION "${PROJECT_VERSION}-${CMAKE_BUILD_TYPE}")
SET (CPACK_DEBIAN_PACKAGE_DEPENDS "socat")
SET (CPACK_GENERATOR DEB)
SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/License.txt)
SET (CPACK_PACKAGE_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

INCLUDE (CPack)
