# CONVERT_PLANTUML_TO_PNG - Parse a list of files and convert the PlantUML files
# to png
#
# CONVERT_PLANTUML_TO_PNG( [RESOURCE_FILES] )
#
# This will run java with the plantuml.jar in headless mode.
FUNCTION (convert_plantuml_to_png plantuml_files_)
  FOREACH (plantuml_file_ ${plantuml_files_})
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
        /usr/bin/java -Djava.awt.headless=true -jar
        /usr/share/plantuml/plantuml.jar -tpng -o ${FILE_DIR} ${plantuml_file_}
      DEPENDS ${plantuml_file_}
      COMMENT "Converting ${plantuml_file_} to ${output_png_}"
      VERBATIM)

    ADD_CUSTOM_TARGET (
      ${FILE_NAME_WE}_png ALL
      DEPENDS ${output_png_}
      COMMENT "Building target: ${FILE_NAME_WE}_png")
  ENDFOREACH ()
ENDFUNCTION ()
