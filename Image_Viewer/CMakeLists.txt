cmake_minimum_required(VERSION 3.10)
project(Image_Viewer)

find_package(ZLIB REQUIRED)
find_package(SDL3 REQUIRED)

add_executable(Image_Viewer src/main.c)
target_link_libraries(Image_Viewer PRIVATE ZLIB::ZLIB SDL3::SDL3)