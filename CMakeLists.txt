cmake_minimum_required(VERSION 3.30)
project(SeaBattleBot)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include your own headers
include_directories(include)

# Find Boost first
find_package(Boost 1.65 REQUIRED COMPONENTS system)

# Set prefix path for TgBot
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/TgBot)

# Find TgBot package
find_package(TgBot REQUIRED)

# Automatically find all .cpp files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(NOT SOURCES)
    message(FATAL_ERROR "No .cpp files found in src/")
endif()

message(STATUS "Source files: ${SOURCES}")

# Create executable
add_executable(SeaBattleBot ${SOURCES})

# Link TgBot library (it will automatically link Boost::system)
target_link_libraries(SeaBattleBot TgBot::TgBot)
