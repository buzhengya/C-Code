#CMake 最低版本要求
cmake_minimum_required (VERSION 2.8)

#变量
set(PROJECT_NAME rbtree)
set(EXE_NAME ${PROJECT_NAME})

#项目信息
project(${PROJECT_NAME})

#编译器
set(CMAKE_C COMPLIER gcc)
set(CMAKE_CXX COMPLIER g++)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11")

# ========== src file =======
aux_source_directory(. DIR_SRCS)
#aux_source_directory(luxnet DIR_SRCS)


#指定生成目标
add_executable(${EXE_NAME} ${DIR_SRCS})
