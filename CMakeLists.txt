cmake_minimum_required(VERSION 3.10)

# 项目信息
project(BookManageSystemV1.3 C)

# 设置C标准
set(CMAKE_C_STANDARD 11)

# 查找GTK库
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# 包含GTK头文件
include_directories(${GTK3_INCLUDE_DIRS})

# 链接GTK库
link_directories(${GTK3_LIBRARY_DIRS})

# 添加定义
add_definitions(${GTK3_CFLAGS_OTHER})

# 源文件
set(SOURCES
        gui.h
        gui.c
        user.c
        user.h
        book.h
        book.c
        gui.c)

# 可执行文件
add_executable(library_management ${SOURCES})

# 链接库
target_link_libraries(library_management ${GTK3_LIBRARIES})
