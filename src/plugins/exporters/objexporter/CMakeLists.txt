set(objexporter_SOURCES
        objexporter.h
        objexporter.cpp
        obj_writer.h
        obj_writer.cpp)

add_library(objexporter SHARED ${objexporter_SOURCES})

target_include_directories(objexporter PRIVATE ../../../)
target_link_libraries(objexporter core Qt5::Core ${OPENGL_LIBRARIES})


set_target_properties(objexporter
                PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/exporters"
                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/exporters"
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/exporters"
                )
