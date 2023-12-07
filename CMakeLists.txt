cmake_minimum_required(VERSION 3.5)

project(smart_pointers)


add_executable(smart_pointers src/main.cpp src/shared_ptr.h src/unique_ptr.h)

set_target_properties(
      smart_pointers PROPERTIES
      CXX_STANDART 11
      CXX_STANDART_REQUIRED ON)