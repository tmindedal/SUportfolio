cmake_minimum_required(VERSION 3.10)
project(SqliteGame)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})  # ← Tilføj denne

add_executable(main 
    gameMain.cpp 
    game.cpp 
    sqlite3.c
)
