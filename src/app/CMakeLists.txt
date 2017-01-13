set(app_SOURCES
        ivygeneratorwindow.cpp
        ui/aboutdialog.cpp
        ui/widgets/birth_widget.cpp
        ui/widgets/colorpickerwidget.cpp
        ui/widgets/pathedit.cpp
        ui/settingsdialog.cpp
        ui/widgets/setup_widget.cpp
        ui/widgets/render_widget.cpp
        ivy.cpp
        common.cpp
        configuration.cpp
        plugin_loader.cpp
        main.cpp
        resources/ivygen.qrc
        resources/ivygen.rc
        ui/aboutdialog.ui
        ui/widgets/colorpickerwidget.ui
        ui/widgets/pathedit.ui
        ui/settingsdialog.ui
)

add_executable(${CMAKE_PROJECT_NAME} ${app_SOURCES})

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_nullptr)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_range_for)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ../ ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} core Qt5::Core Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES})

set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

IF(MSVC)
    set_target_properties(${CMAKE_PROJECT_NAME}
            PROPERTIES
                WIN32_EXECUTABLE YES
                LINK_FLAGS "/ENTRY:mainCRTStartup"
                    )
ENDIF(MSVC)
