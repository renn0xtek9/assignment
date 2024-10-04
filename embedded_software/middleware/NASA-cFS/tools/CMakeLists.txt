# CMake snippet for building the host-side tools.
PROJECT (CFETOOLS C)

ADD_SUBDIRECTORY (cFS-GroundSystem/Subsystems/cmdUtil)
ADD_SUBDIRECTORY (elf2cfetbl)
# add_subdirectory(tblCRCTool)
