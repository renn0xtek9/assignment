EXECUTE_PROCESS (
  COMMAND git rev-parse --short HEAD
  RESULT_VARIABLE SHORT_HASH_RESULT
  OUTPUT_VARIABLE SHORT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

# If running in script mode (this runs on every build)
IF (CMAKE_SCRIPT_MODE_FILE)
  IF (EXISTS "${SHORT_HASH_FILE}")
    FILE (READ ${SHORT_HASH_FILE} READ_IN_SHORT_HASH)
  ELSE ()
    SET (READ_IN_SHORT_HASH "")
  ENDIF ()

  IF (NOT ("${READ_IN_SHORT_HASH}" STREQUAL "${SHORT_HASH}"))
    MESSAGE (STATUS "Short hash is out of date")
    # This will update short_hash.txt, causing cmake to reconfigure
    FILE (WRITE ${SHORT_HASH_FILE} ${SHORT_HASH})
  ENDIF ()

  # Else running as part of cmake configure
ELSE ()
  SET (SHORT_HASH_FILE ${CMAKE_CURRENT_BINARY_DIR}/short_hash.txt)
  FILE (WRITE ${SHORT_HASH_FILE} ${SHORT_HASH})

  # The trick here is to make sure short_hash.txt is listed as a byproduct
  ADD_CUSTOM_TARGET (
    git_short_hash
    BYPRODUCTS ${SHORT_HASH_FILE}
    COMMAND ${CMAKE_COMMAND} "-DSHORT_HASH_FILE=${SHORT_HASH_FILE}" "-P"
            "${CMAKE_CURRENT_LIST_FILE}"
    COMMENT "Re-checking short hash..."
    VERBATIM USES_TERMINAL)

  # This configure_file makes cmake reconfigure dependent on short_hash.txt
  CONFIGURE_FILE (${SHORT_HASH_FILE} ${SHORT_HASH_FILE}.junk COPYONLY)
  CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
                  ${CMAKE_BINARY_DIR}/version.hpp)

  MESSAGE (STATUS "Short Hash: ${SHORT_HASH}")
ENDIF ()
