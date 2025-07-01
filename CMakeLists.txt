cmake_minimum_required (VERSION 3.10...3.35)

project (Ursa)

add_subdirectory ("vendor/glad-02.00")
add_subdirectory ("vendor/glfw-03.04")
add_subdirectory ("vendor/spdlog-01.15")

add_definitions (-DGLFW_INCLUDE_NONE)

add_executable (Ursa
  "source/Main.cpp"
)

target_link_libraries (Ursa PRIVATE glad glfw spdlog::spdlog)
