cmake_minimum_required(VERSION 3.5)

project(SimpleCamera)

if( MINGW )
    if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O0 -g -Wall -Wunused-function -pthread")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -pthread")
        message("MINGW - Debug mode")
    elseif( CMAKE_BUILD_TYPE STREQUAL "Release" ) # debug
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -Wunused-function")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O2 -pthread")
        message("MINGW - Release mode")
    endif() # Debug
    #
    if( PROJECT_ARCHITECTURE STREQUAL "x64" ) # x64
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    elseif(PROJECT_ARCHITECTURE STREQUAL "x86") # x86
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    endif() # x64
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} " )
    set( CMAKE_EXECUTABLE_SUFFIX ".exe" )
endif() # MINGW

if( APPLE )
    if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g -Wall -Wunused-function -pthread")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -pthread")
        message("CLANG - Debug mode")
    elseif( CMAKE_BUILD_TYPE STREQUAL "Release" ) # debug
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -Wunused-function")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O2 -pthread")
        message("CLANG - Release mode")
    endif() # Debug
    #
    if( PROJECT_ARCHITECTURE STREQUAL "x64" ) # x64
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    elseif(PROJECT_ARCHITECTURE STREQUAL "x86") # x86
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    endif() # x64
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} " )
endif() # CLANG

# Microsoft Visual C++ Compilers
if(MSVC)
    if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast /Gy /Oi /Oy /O2 /Ot /Zi /EHsc ")
    endif()
    if(CMAKE_CL_64)
        set( PROJECT_ARCHITECTURE "x64" )
    else(CMAKE_CL_64)
        set( PROJECT_ARCHITECTURE "x86" )
    endif(CMAKE_CL_64)
    #
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    #
    set( CMAKE_EXECUTABLE_SUFFIX ".exe" )
endif()

#设置输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#设定库文件后缀名
set(CMAKE_DEBUG_POSTFIX "d")

#添加项目组件
add_subdirectory(src)
