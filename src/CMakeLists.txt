cmake_minimum_required(VERSION 3.22)
project(Simple_TTT)

set(CMAKE_CXX_STANDARD 17)

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../lib/raylib/src/linux/libraylib.a)

add_library(imgui STATIC IMPORTED)
set_target_properties(imgui PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../lib/imgui/libimgui.a)

add_executable(Simple_TTT main.cpp gui.cpp server.cpp client.cpp bot.cpp shapes.cpp gameplay.cpp)

target_link_libraries(Simple_TTT raylib GL imgui)
