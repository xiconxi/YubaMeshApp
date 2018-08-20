set(CMAKE_INCLUDE_CURRENT_DIR  ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(LIB_NAME Measurement)
add_library(Measurement SHARED
        measurement_backend.cpp measurement_backend.h
        measurement_plugin.cpp measurement_plugin.h)
SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../${LIB_NAME}/)
target_link_libraries(${LIB_NAME} Qt5::Core Qt5::Quick )
add_custom_command(TARGET ${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/qml $<TARGET_FILE_DIR:${LIB_NAME}>)