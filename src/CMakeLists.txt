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

  ConcurrentQueue.h
  CommonTypes.h
  FieldDrawer.h)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
