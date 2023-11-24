cmake_minimum_required(VERSION 3.5)

project(my_libco-test)

include_directories(
  ${PROJECT_SOURCE_DIR}/../include
)

link_directories(
  ${PROJECT_SOURCE_DIR}/../build/lib
)

aux_source_directory(. src_list)

set(CMAKE_EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
message("*******${CMAKE_EXECUTABLE_OUTPUT_PATH}********")

add_executable(${PROJECT_NAME} ${src_list})

target_link_libraries(
  ${PROJECT_NAME}
  my_libco dl
)
