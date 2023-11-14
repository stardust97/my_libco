cmake_minimum_required(VERSION 3.5)

project(my_libco)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

link_directories(
  ${PROJECT_SOURCE_DIR}/build/lib
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
