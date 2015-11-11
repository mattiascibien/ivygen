cmake_minimum_required (VERSION 2.8.11)
project (IvyGen)

add_subdirectory (src)

install (DIRECTORY data/ DESTINATION data/)
install (DIRECTORY textures/ DESTINATION textures/)