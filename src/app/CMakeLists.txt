set(app_SOURCES 
	ivygeneratorwindow.cpp
	aboutdialog.cpp
	birthwidget.cpp
	colorpickerwidget.cpp
	settingsdialog.cpp
	SetupWidget.cpp
	RenderWidget.cpp
	Ivy.cpp
	Common.cpp
	configuration.cpp
	main.cpp
	resources/ivygen.qrc)

add_executable(${CMAKE_PROJECT_NAME} ${app_SOURCES})

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_nullptr)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ../core)
target_link_libraries(${CMAKE_PROJECT_NAME} core Qt5::Core Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)