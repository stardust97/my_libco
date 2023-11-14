aux_source_directory(. src_list)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
enable_language(ASM)

add_library(${PROJECT_NAME} SHARED ${src_list} ${PROJECT_SOURCE_DIR}/src/coctx_swap.S)