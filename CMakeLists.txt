cmake_minimum_required (VERSION 3.10...3.35)

project (Ursa)

add_subdirectory ("vendor/glad-02.00")
add_subdirectory ("vendor/glfw-03.04")
add_subdirectory ("vendor/spdlog-01.15")
add_subdirectory ("vendor/stb-image-02.30")

add_definitions (-DGLFW_INCLUDE_NONE)

add_executable (Ursa
  "source/Log.cpp"
  "source/Main.cpp"
  "source/Shader.cpp"

  "source/Log.hpp"
  "source/Shader.hpp"
)

set_property (TARGET Ursa PROPERTY CXX_STANDARD 17)
set_property (TARGET Ursa PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries (Ursa PRIVATE glad glfw spdlog::spdlog stb_image)
