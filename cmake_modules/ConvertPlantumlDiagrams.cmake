INCLUDE (CMakeParseArguments)
# CONVERT_PLANTUML_TO_PNG - Parse a list of files and convert the PlantUML files
# to png
#
# CONVERT_PLANTUML_TO_PNG( PLANTUML_FILES [RESOURCE_FILES] DOCUMENT [DEPENDENT
# DOCUMENT])
#
# This will run java with the plantuml.jar in headless mode.
FUNCTION (convert_plantuml_to_png)
  CMAKE_PARSE_ARGUMENTS (
    PARSED_ARGS # prefix of output variables
    "" # list of names of the boolean arguments (only defined ones will be true)
    "" # list of names of mono-valued arguments
    "PLANTUML_FILES;DOCUMENT" # list of names of multi-valued arguments (output
    # variables are lists)
    ${ARGN} # arguments of the function to parse, here we take the all original
            # ones
  )
  IF (NOT PARSED_ARGS_PLANTUML_FILES)
    MESSAGE (FATAL_ERROR "You must provide the argumment PLANTUML_FILES")
  ENDIF (NOT PARSED_ARGS_PLANTUML_FILES)

  FIND_PROGRAM (JAVA java)
  FOREACH (plantuml_file_ ${PARSED_ARGS_PLANTUML_FILES})
    GET_FILENAME_COMPONENT (FILE_EXTENSION ${plantuml_file_} EXT)

    IF (NOT "${FILE_EXTENSION}" STREQUAL ".plantuml")
      CONTINUE ()
    ENDIF ()

    GET_FILENAME_COMPONENT (FILE_NAME_WE ${plantuml_file_} NAME_WE)
    GET_FILENAME_COMPONENT (FILE_DIR ${plantuml_file_} DIRECTORY)

    SET (output_png_ "${FILE_DIR}/${FILE_NAME_WE}.png")

    ADD_CUSTOM_COMMAND (
      OUTPUT ${output_png_}
      COMMAND
        ${JAVA} -Djava.awt.headless=true -jar /usr/share/plantuml/plantuml.jar
        -tpng -o ${FILE_DIR} ${plantuml_file_}
      DEPENDS ${plantuml_file_}
      COMMENT "Converting ${plantuml_file_} to ${output_png_}"
      VERBATIM)

    ADD_CUSTOM_TARGET (
      ${FILE_NAME_WE}_png ALL
      DEPENDS ${output_png_}
      COMMENT "Building target: ${FILE_NAME_WE}_png")

    IF (PARSED_ARGS_DOCUMENT)
      ADD_DEPENDENCIES (${PARSED_ARGS_DOCUMENT} ${FILE_NAME_WE}_png)
    ENDIF (PARSED_ARGS_DOCUMENT)
  ENDFOREACH ()
ENDFUNCTION ()
