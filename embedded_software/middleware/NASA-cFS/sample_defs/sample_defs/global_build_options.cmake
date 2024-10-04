#
# Example global_build_options.cmake
# ----------------------------------
#
# This may set global definitions that apply to ALL targets in ALL scopes,
# including FSW code that is cross-compiled for a target as well as code built
# for the development host itself (native).
#
# As such, it should only invoke basic commands that have wide applicability,
# such as "add_definitions()" for macro definitions that should be set globally.
# It should not include any compiler-specific options that might change between
# compiler vendors or target processor families.
#

# If the OMIT_DEPRECATED flag is specified, then define the respective macros
# that omit the deprecated features from the build.  This is conditional in this
# example for CI purposes, so it can be tested both ways.  Most projects would
# likely set this only one way.
SET (
  OMIT_DEPRECATED
  $ENV{OMIT_DEPRECATED}
  CACHE STRING "Omit deprecated elements")
IF (OMIT_DEPRECATED)
  MESSAGE (
    STATUS "OMIT_DEPRECATED=true: Not including deprecated elements in build")
  ADD_DEFINITIONS (-DCFE_OMIT_DEPRECATED_6_8 -DCFE_OMIT_DEPRECATED_6_7
                   -DCFE_OMIT_DEPRECATED_6_6 -DOSAL_OMIT_DEPRECATED)
  SET (MISSION_RESOURCEID_MODE "STRICT") # more type safe, but less backward
                                         # compatible
ELSE ()
  MESSAGE (
    STATUS "OMIT_DEPRECATED=false: Deprecated elements included in build")
  SET (MISSION_RESOURCEID_MODE "SIMPLE") # less type safe, but more backward
                                         # compatible
ENDIF (OMIT_DEPRECATED)
