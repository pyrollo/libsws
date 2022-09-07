cmake_minimum_required(VERSION 3.14)

project(libsws LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sws STATIC
  swsengine.h swsengine.cpp
  swsvalue.h
  swsexception.h
  swsplug.h swsplug.cpp
  swsmodule.h swsmodule.cpp
  swsfactory.h
  swscontainer.h swscontainer.cpp
  swscontainermodule.h swscontainermodule.cpp
  atoms/baseoperations.h atoms/baseoperations.cpp
)

target_compile_definitions(sws PRIVATE LIBSWS_LIBRARY)
