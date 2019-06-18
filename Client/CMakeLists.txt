cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 14)
include_directories(include)

add_executable(Client main.cpp include/Client.h include/Fsm.h include/BaseSession.h include/Options.h src/Client.cpp)