cmake_minimum_required(VERSION 2.6)

include_directories(inc)

add_library( soil STATIC
  src/image_helper.c
  src/stb_image_aug.c
  src/image_DXT.c
  src/SOIL.c)

message("HEY")