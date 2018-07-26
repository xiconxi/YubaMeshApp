cmake_minimum_required(VERSION 3.5)
project(YubaMs)
if (UNIX)
    set(Qt5_DIR "/home/user/Qt5.10.0/5.10.0/gcc_64/lib/cmake/Qt5")
endif (UNIX)
include_directories(./depends)
find_package(Qt5 COMPONENTS Core Quick REQUIRED)
add_subdirectory(plugins)
add_subdirectory(app)


