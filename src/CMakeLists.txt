cmake_minimum_required (VERSION 2.8.11)
project (IvyGen)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenGL)

# Find the QtCore library
find_package(Qt5Core)

# Find the QtCore library
find_package(Qt5OpenGL)

# Find the QtWidgets library
find_package(Qt5Widgets)

add_subdirectory (core)
add_subdirectory (app)
