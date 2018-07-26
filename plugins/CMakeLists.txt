add_subdirectory(glviewer)
add_subdirectory(measurement)
add_subdirectory(meshMorph)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pluginsConfig.xml ${PROJECT_BINARY_DIR}/pluginsConfig.xml COPYONLY)