cmake_minimum_required(VERSION 3.5)

project(MainUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

#使用findpackage的方式加入halcon,这个方式用的很差
#if(DEFINED ENV{HALCONROOT} AND NOT $ENV{HALCONROOT} STREQUAL "")
#  message("Using HALCONROOT " $ENV{HALCONROOT})
#  STRING(REGEX REPLACE "\\\\" "/" HALCONROOT $ENV{HALCONROOT})
#endif()
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
#find_package(Halcon REQUIRED)
#find_package(HDevEngine REQUIRED)
#include_directories(${HALCON_INC_DIRS})
#link_directories(${HALCON_LIBS})

#使用路径添加的方式jia'r加入halcon
#加入halcon头文件路径
 include_directories($ENV{HALCONROOT}/include)
#加入halcon lib依赖
 link_directories($ENV{HALCONROOT}/lib/$ENV{HALCONARCH})


file(GLOB Headers *.h)
file(GLOB Cpps *.cpp)
file(GLOB UIs *.ui)
file(GLOB Src *.qrc)
if(ANDROID)
  add_library(MainUI SHARED
    ${Headers}
    ${Cpps}
    ${UIs}
    ${Src}
  )
else()
  add_executable(MainUI
    ${Headers}
    ${Cpps}
    ${UIs}
    ${Src}
  )
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC halconcpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
