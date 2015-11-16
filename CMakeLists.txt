cmake_minimum_required (VERSION 2.8.11)
project (IvyGen)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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

add_subdirectory (src)

install (DIRECTORY data/ DESTINATION data/)
install (DIRECTORY textures/ DESTINATION textures/)
install (DIRECTORY ${CMAKE_BINARY_DIR}/bin/ DESTINATION bin/)
install (DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION lib/)
install (DIRECTORY ${CMAKE_BINARY_DIR}/plugins/ DESTINATION plugins/)
