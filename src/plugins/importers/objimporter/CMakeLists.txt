set(objimporter_SOURCES
        objimporter.h
        objimporter.cpp
        objloader/list.h
        objloader/list.cpp
        objloader/obj_parser.h
        objloader/obj_parser.cpp
        objloader/objLoader.h
        objloader/objLoader.cpp
        objloader/string_extra.h
        objloader/string_extra.cpp)

add_library(objimporter SHARED ${objimporter_SOURCES})

target_include_directories(objimporter PRIVATE ../../../core)
target_link_libraries(objimporter Qt5::Core ${OPENGL_LIBRARIES})


set_target_properties(objimporter
                PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/importers"
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/importers"
                )
