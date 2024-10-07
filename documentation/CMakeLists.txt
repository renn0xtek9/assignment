PROJECT (
  developer-documentation
  VERSION ${GLOBAL_VERSION}
  LANGUAGES NONE)
ADD_SUBDIRECTORY (requirements_engineering)
ADD_SUBDIRECTORY (build_pipeline_documentation)
ADD_SUBDIRECTORY (doxygen)
ADD_SUBDIRECTORY (development_process)

ADD_CUSTOM_TARGET (
  documentation ALL
  COMMENT "Documentation"
  DEPENDS deliverables_documentation code_documentation
          build_pipeline_documentation software_build_process_documentation)
