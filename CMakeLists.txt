cmake_minimum_required(VERSION 3.5)
project(WebNote)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/Handlers)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/dist)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIR_SRCS)
add_executable(WebNote ${DIR_SRCS})

target_link_libraries(WebNote hiredis boost_system boost_filesystem pthread)