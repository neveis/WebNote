cmake_minimum_required(VERSION 3.5)
project(WebNote)

include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIR_SRCS)
add_executable(WebNote ${DIR_SRCS})
#set(SOURCE_FILES main.cpp)
#add_executable(WebNote ${SOURCE_FILES})

target_link_libraries(WebNote hiredis boost_system boost_filesystem pthread)