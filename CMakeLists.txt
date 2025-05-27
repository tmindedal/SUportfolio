# Minimum version af CMake
cmake_minimum_required(VERSION 3.10)

# Projektets navn
project(HeroGame)

# Brug C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Gør klart hvilke filer vi bygger
add_executable(main
    gameMain.cpp
    game.cpp
    sqlite3.c  # C-kode
)

# Fortæl CMake at sqlite3.c er C, ikke C++
set_source_files_properties(sqlite3.c PROPERTIES LANGUAGE C)
