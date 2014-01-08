cmake_minimum_required(VERSION 3.0)
project(ExprAdvance)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE)
add_executable(${PROJECT_NAME} ${SOURCE})
