cmake_minimum_required(VERSION 3.10)
project(SqliteGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

add_executable(main 
    gameMain.cpp 
    game.cpp
)

target_link_libraries(main PRIVATE SQLite::SQLite3 pthread dl)
