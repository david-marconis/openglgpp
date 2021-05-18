project(openglcpp)
cmake_minimum_required(VERSION 3.0)
set(CONAN_SYSTEM_INCLUDES ON)

add_definitions("-std=c++20")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(src, include)
if(BASIC)
  set(PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/basic/${MAIN_CLASS}")
else()
  file(GLOB PROJECT_SOURCE CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
  list(APPEND PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/programs/${MAIN_CLASS}")
endif()
add_executable(openglcpp ${PROJECT_SOURCE})
target_link_libraries(openglcpp ${CONAN_LIBS})
