cmake_minimum_required(VERSION 3.5)

#加入公共头文件
include_directories(${CMAKE_SOURCE_DIR}/include)

#加入公共引用库
link_directories(${CMAKE_SOURCE_DIR}/lib)

#设置环境变量
file(GLOB AllDlls ${CMAKE_SOURCE_DIR}/bin/*.dll)
link_libraries(${AllDlls})

#添加组件
add_subdirectory(MainUI)

#设定Debug模式下的exe输出带有d后缀
set_target_properties(MainUI PROPERTIES DEBUG_POSTFIX "d")