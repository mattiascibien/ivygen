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

target_link_libraries(core Qt5::Core Qt5::OpenGL ${OPENGL_LIBRARIES})

install(TARGETS core
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)