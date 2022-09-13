cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 20)

project("ShaderApplicator")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS graphics REQUIRED)

add_executable(ShaderApplicator main.cpp)
target_link_libraries(ShaderApplicator sfml-graphics)