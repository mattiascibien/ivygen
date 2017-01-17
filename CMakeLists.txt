cmake_minimum_required (VERSION 3.0)
project (IvyGen)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(GetGitRevisionDescription)

if(DEFINED $ENV{APPVEYOR_BUILD_VERSION})
    message(STATUS "Using appveyor version = " $ENV{APPVEYOR_BUILD_VERSION})
    set(VERSION $ENV{APPVEYOR_BUILD_VERSION})
else()
    git_describe(VERSION --tags)
    message(STATUS "Using local version = " ${VERSION})
endif()




#parse the version information into pieces.
string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")

string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

IF (WIN32)
    SET(APPS "bin/${CMAKE_PROJECT_NAME}.exe")
    SET(DIRS ${QT_LIBRARY_DIRS})
    INSTALL(CODE "
        file(GLOB_RECURSE QTPLUGINS
          \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        include(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
        " COMPONENT Runtime)
ENDIF(WIN32)

IF (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(MSVC)

add_subdirectory (src)

if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
	include(WinDeployQt)
	WinDeployQt(TARGET ${CMAKE_PROJECT_NAME} COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS})
	install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/ DESTINATION bin/)
endif()

install (DIRECTORY data/ DESTINATION data/)
install (DIRECTORY textures/ DESTINATION textures/)
install (DIRECTORY ${CMAKE_BINARY_DIR}/bin/ DESTINATION bin/)
install (DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION lib/)
install (DIRECTORY ${CMAKE_BINARY_DIR}/plugins/ DESTINATION plugins/)

set (CPACK_PACKAGE_VENDOR "Mattias Cibien")
set (CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
include(CPack)
