set(core_SOURCES 
        plugins/importer_interface.h
        basic_mesh.cpp
        camera.cpp
        model_loader.cpp
        obj_writer.cpp)

add_library(core SHARED ${core_SOURCES})

target_compile_features(core PRIVATE cxx_nullptr)
target_compile_definitions(core PRIVATE -DCORE_LIBRARY)
target_link_libraries(core Qt5::Core Qt5::OpenGL ${OPENGL_LIBRARIES})


# Should not use this
target_include_directories(core PRIVATE .. ${Qt5Core_INCLUDE_DIRS})
qt5_use_modules(core Core)
qt5_use_modules(core OpenGL)

set_target_properties(core
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		)
