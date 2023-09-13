# 指定最低的CMake版本
cmake_minimum_required(VERSION 3.0)

# 使用 C++14 标准
set(CMAKE_CXX_STANDARD 14)

# 设置编译器
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# 设定项目名称
project(BookStore)
# 外部库搜索目录 -DCMAKE_LIBRARY_PATH=/usr/local/lib,这句话不要写两次否则，只在第2次的位置生效
link_directories(/usr/local/lib)

# 创建client执行文件
add_executable(client client.cpp)
# 告诉编译器在哪里找到外部头文件: -I参数
include_directories(client /usr/local/include /usr/local/include/luajit-2.1  ./LuaBridge/Source)
# 将外部库链接到可执行文件中: -l参数
target_link_libraries(client PRIVATE dl zmq pthread luajit-5.1)

# 创建server可执行文件
add_executable(server server.cpp)
# 告诉编译器在哪里找到外部头文件: -I参数
include_directories(server /usr/local/include /usr/local/include/luajit-2.1 ./LuaBridge/Source)
# 将外部库链接到可执行文件中: -l参数
target_link_libraries(server PRIVATE dl zmq pthread luajit-5.1)

# 创建共享库,可以在其他的CMakeLists.txt文件中使用target_link_libraries将这个库链接到其他可执行文件中
#add_library(bookstore client.cpp conf.lua config.hpp myzmq.hpp sales_data.hpp server.cpp spin_lock.hpp summary.hpp)

# 安装库文件/可执行文件
#install (TARGETS bookstore DESTINATION bin)

# 安装头文件
#install (FILES config.hpp myzmq.hpp sales_data.hpp spin_lock.hpp summary.hpp DESTINATION include)

# 添加一个自定义的清理目标
# add_custom_target(clean
#     COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/clean-all.cmake
#     COMMENT "Cleaning up..."
# )