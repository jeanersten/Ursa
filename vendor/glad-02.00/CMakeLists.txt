cmake_minimum_required(VERSION 3.10)

project(glad LANGUAGES C)

add_library(glad STATIC
  "src/gl.c"

  "include/glad/gl.h"
  "include/KHR/khrplatform.h"
)

target_include_directories(glad PUBLIC include)
