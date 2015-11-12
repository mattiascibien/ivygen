set(core_SOURCES 
	BasicMesh.cpp
	Camera.cpp
	ModelLoader.cpp
	OBJWriter.cpp
	objloader/list.cpp
	objloader/obj_parser.cpp
	objloader/objLoader.cpp
	objloader/string_extra.cpp)

add_library(core SHARED ${core_SOURCES})

target_compile_definitions(core PRIVATE -DCORE_LIBRARY)
target_link_libraries(core Qt5::Core Qt5::OpenGL ${OPENGL_LIBRARIES})

set_target_properties(core
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		)