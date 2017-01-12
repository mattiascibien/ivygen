IF(VERSION)
    add_definitions(-DGIT_VERSION="${VERSION}")
ENDIF(VERSION)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenGL REQUIRED)

# Find the QtCore library
find_package(Qt5Core REQUIRED)

# Find the QtCore library
find_package(Qt5OpenGL REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

add_subdirectory (core)
add_subdirectory (app)
add_subdirectory (plugins)
