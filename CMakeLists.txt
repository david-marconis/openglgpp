project(openglcpp)
cmake_minimum_required(VERSION 3.0)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(src)
set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/${MAIN_CLASS}")
add_executable(openglcpp ${PROJECT_MAIN})
target_link_libraries(openglcpp ${CONAN_LIBS})
