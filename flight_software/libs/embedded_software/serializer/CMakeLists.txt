SET (LIBNAME serializer)
CREATE_LIBRARY (SRCS src/serializer.cpp PUBLIC_HEADERS
                include/${LIBNAME}/serializer.h LINK_LIBRARIES messages)

ADD_SUBDIRECTORY (tests)
