ADD_SUBDIRECTORY (requirements_engineering)
ADD_SUBDIRECTORY (doxygen)
ADD_SUBDIRECTORY (development_process)

ADD_CUSTOM_TARGET (
  documentation ALL
  COMMENT "Documentation"
  DEPENDS deliverables_documentation code_documentation
          software_build_process_documentation)
