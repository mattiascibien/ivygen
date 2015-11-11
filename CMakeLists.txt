cmake_minimum_required (VERSION 2.8.11)
project (IvyGen)

add_subdirectory (src)

install (DIRECTORY data/ DESTINATION data/)
install (DIRECTORY textures/ DESTINATION textures/)


IF (WIN32)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_PROJECT_NAME}.exe")
SET(DIRS ${QT_LIBRARY_DIRS})
INSTALL(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)
ENDIF(WIN32)