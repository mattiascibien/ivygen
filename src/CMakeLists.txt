IF(VERSION)
    add_definitions(-DGIT_VERSION="${VERSION}")
ENDIF(VERSION)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenGL)

# Find the QtCore library
find_package(Qt5Core)

# Find the QtCore library
find_package(Qt5OpenGL)

# Find the QtWidgets library
find_package(Qt5Widgets)

add_subdirectory (core)
add_subdirectory (app)
add_subdirectory (plugins)
