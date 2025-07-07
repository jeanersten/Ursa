cmake_minimum_required (VERSION 3.10...3.15)

project (stb)

add_library (stb_image STATIC
  "src/stb_image.c"
  "include/stb_image.h"
)

target_include_directories (stb_image PUBLIC include)
