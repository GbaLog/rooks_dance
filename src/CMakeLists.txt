cmake_minimum_required(VERSION 3.10)

project(chess_board)

set(CMAKE_CXX_STANDARD 17)

set(SRC_LIST
  main.cpp
  Rook.h
  RookHandler.cpp
  RookHandler.h
  BoardMng.cpp
  BoardMng.h

  RandUtils.cpp
  RandUtils.h
  ConcurrentQueue.h
  CommonTypes.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})
