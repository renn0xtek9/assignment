# This example toolchain file describes the cross compiler to use for the target
# architecture indicated in the configuration file.

# Basic cross system configuration
SET (CMAKE_SYSTEM_NAME VxWorks-CFE)
SET (CMAKE_SYSTEM_PROCESSOR ppc)
SET (CMAKE_SYSTEM_VERSION 6.9)
SET (VXWORKS_GCC_VERSION 4.3.3)
SET (VXWORKS_HOST_VERSION x86-linux2)

# The VxWorks toolchain relies on several environment variables, which should be
# set already by an environment setup script.

# WIND_HOME refers to the top-level installation directory
SET (WIND_HOME_DFL "$ENV{WIND_HOME}")
IF (NOT WIND_HOME_DFL)
  SET (WIND_HOME_DFL "/opt/WindRiver")
ENDIF (NOT WIND_HOME_DFL)
SET (
  WIND_HOME
  "${WIND_HOME_DFL}"
  CACHE PATH "Wind River top level installation directory")

# WIND_BASE refers to the system-specific installation directory (e.g.
# vxworks-<VERSION>)
SET (WIND_BASE_DFL "$ENV{WIND_BASE}")
IF (NOT WIND_BASE_DFL)
  SET (WIND_BASE_DFL "${WIND_HOME}/vxworks-${CMAKE_SYSTEM_VERSION}")
ENDIF (NOT WIND_BASE_DFL)
SET (
  WIND_BASE
  "${WIND_BASE_DFL}"
  CACHE PATH "Wind River product installation directory")

SET (
  VXWORKS_TOOLS_PREFIX
  "${WIND_HOME}/gnu/${VXWORKS_GCC_VERSION}-vxworks-${CMAKE_SYSTEM_VERSION}/${VXWORKS_HOST_VERSION}"
)

# specify the cross compiler - adjust accord to compiler installation This uses
# the compiler-wrapper toolchain that buildroot produces
SET (SDKHOSTBINDIR "${VXWORKS_TOOLS_PREFIX}/bin")
SET (TARGETSUFFIX "${CMAKE_SYSTEM_PROCESSOR}")
# set(VXWORKS_BSP_C_FLAGS           "-march=i686 -mtune=i686 -fno-common")
# set(VXWORKS_BSP_CXX_FLAGS         ${VXWORKS_BSP_C_FLAGS})

SET (CMAKE_C_COMPILER "${SDKHOSTBINDIR}/cc${TARGETSUFFIX}")
SET (CMAKE_CXX_COMPILER "${SDKHOSTBINDIR}/c++${TARGETSUFFIX}")
SET (CMAKE_LINKER "${SDKHOSTBINDIR}/ld${TARGETSUFFIX}")
SET (CMAKE_ASM_COMPILER "${SDKHOSTBINDIR}/as${TARGETSUFFIX}")
SET (CMAKE_AR "${SDKHOSTBINDIR}/ar${TARGETSUFFIX}")
SET (CMAKE_OBJDUMP "${SDKHOSTBINDIR}/objdump${TARGETSUFFIX}")
SET (CMAKE_RANLIB "${SDKHOSTBINDIR}/ranlib${TARGETSUFFIX}")

# search for programs in the build host directories
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET (CMAKE_PREFIX_PATH /)

# these settings are specific to cFE/OSAL and determines which abstraction
# layers are built when using this toolchain
SET (CFE_SYSTEM_PSPNAME mcp750-vxworks)

INCLUDE_DIRECTORIES (${WIND_BASE}/target/h/wrn/coreip)
INCLUDE_DIRECTORIES (${WIND_BASE}/target/h)
