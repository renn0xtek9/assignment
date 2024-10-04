#
# Example arch_build_custom.cmake
# -------------------------------
#
# On native builds only, add strict cast alignment warnings This requires a
# newer version of gcc
#
ADD_COMPILE_OPTIONS (
  -Wcast-align=strict # Warn about casts that increase alignment requirements
  -fno-common # Do not use a common section for globals
)
