cmake_minimum_required(VERSION 3.15)
project(PokerAI)

set(CMAKE_CXX_STANDARD 14)

add_executable(PokerAI main.cpp game.cpp game.h cards.h cards.cpp pokerAI.cpp pokerAI.h)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(PokerAI glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(PokerAI OpenGL::GL)