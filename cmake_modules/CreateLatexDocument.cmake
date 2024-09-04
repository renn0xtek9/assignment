INCLUDE (Colors)
INCLUDE (ConvertPlantumlDiagrams)
# CREATE_LATEX_DOCUMENT - Create a latex document
#
# This function supposes that the executable pdflatex, biber and makeindex are
# in the PATH
#
# CREATE_LATEX_DOCUMENT( [PDFNAME TEXSRCS RESOURCEFILES BIBFILE] )
#
# This will run pdflatex several time to re-index the document as needed
MACRO (CREATE_LATEX_DOCUMENT docname_ texsrc_ resources_ bibfile_)
  FIND_PROGRAM (BIBER biber)
  FIND_PROGRAM (PDFLATEX pdflatex)
  FIND_PROGRAM (MAKEINDEX makeindex)

  IF (${BIBER}-NOTFOUND)
    MESSAGE (FATAL_ERROR "biber not found!")
  ENDIF ()
  IF (${PDFLATEX}-NOTFOUND)
    MESSAGE (FATAL_ERROR "pdflatex not found!")
  ENDIF ()
  IF (${MAKEINDEX}-NOTFOUND)
    MESAGE (FATAL_ERROR "pdflatex not found!")
  ENDIF ()
  SET (OUTPUT_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/${docname_}/")
  FILE (MAKE_DIRECTORY ${OUTPUT_FOLDER})

  CONVERT_PLANTUML_TO_PNG ("${resources_}")

  ADD_CUSTOM_TARGET (
    ${docname_}
    COMMENT "Building ${docname_}.pdf"
    DEPENDS ${OUTPUT_FOLDER}/${docname_}.pdf
    SOURCES ${texsrc_})

  ADD_CUSTOM_COMMAND (
    OUTPUT ${OUTPUT_FOLDER}/${docname_}.pdf
    DEPENDS ${texsrc_} ${resources_} ${bibfile_}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT
      "Generating ${docname_}. See ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log"
    VERBATIM
    COMMAND
      echo ${MAGENTA}${PDFLATEX} -halt-on-error -interaction nonstopmode
      -output-directory ${OUTPUT_FOLDER}
      ${CMAKE_CURRENT_SOURCE_DIR}/${docname_}.tex ${COLOURRESET}
    COMMAND
      ${PDFLATEX} -halt-on-error -interaction nonstopmode -output-directory
      ${OUTPUT_FOLDER} ${CMAKE_CURRENT_SOURCE_DIR}/${docname_}.tex >
      ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log
    COMMAND echo ${MAGENTA} ${BIBER} --input-directory ${OUTPUT_FOLDER}
            --output-directory ${OUTPUT_FOLDER} ${docname_}.bcf ${COLOURRESET}
    COMMAND
      ${BIBER} --input-directory ${OUTPUT_FOLDER} --output-directory
      ${OUTPUT_FOLDER} ${docname_}.bcf >>
      ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log
    COMMAND echo "${MAGENTA}cd ${OUTPUT_FOLDER} &&"
            "${MAKEINDEX} ${docname_}.idx${COLOURRESET}"
    COMMAND cd ${OUTPUT_FOLDER} && ${MAKEINDEX} ${docname_}.idx >>
            ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log
    COMMAND
      echo "${MAGENTA}cd ${OUTPUT_FOLDER} &&"
      "${MAKEINDEX} ${docname_}.nlo -s nomencl.ist"
      " -o ${docname_}.nls ${COLOURRESET}"
    COMMAND cd ${OUTPUT_FOLDER} && ${MAKEINDEX} ${docname_}.nlo -s nomencl.ist
            -o ${docname_}.nls >> ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log
    COMMAND
      echo "${MAGENTA}cd ${OUTPUT_FOLDER} &&"
      "${MAKEINDEX} -s ${docname_}.ist -t ${docname_}.glg"
      " -o ${docname_}.gls ${docname_}.glo ${COLOURRESET}"
    COMMAND
      cd ${OUTPUT_FOLDER} && ${MAKEINDEX} -s ${docname_}.ist -t ${docname_}.glg
      -o ${docname_}.gls ${docname_}.glo >>
      ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log
    COMMAND
      echo "${MAGENTA}cd ${OUTPUT_FOLDER} &&"
      "${MAKEINDEX} -s ${docname_}.ist -t ${docname_}.alg"
      " -o ${docname_}.acr ${docname_}.acn ${COLOURRESET}"
    COMMAND
      cd ${OUTPUT_FOLDER} && ${MAKEINDEX} -s ${docname_}.ist -t ${docname_}.alg
      -o ${docname_}.acr ${docname_}.acn >>
      ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log
    COMMAND
      echo "${MAGENTA}${PDFLATEX} -output-directory ${OUTPUT_FOLDER}"
      " -halt-on-error -interaction nonstopmode"
      " ${CMAKE_CURRENT_SOURCE_DIR}/${docname_}.tex ${COLOURRESET}"
    COMMAND
      ${PDFLATEX} -output-directory ${OUTPUT_FOLDER} -halt-on-error
      -interaction nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/${docname_}.tex >>
      ${CMAKE_CURRENT_BINARY_DIR}/${docname_}.log)

ENDMACRO ()
