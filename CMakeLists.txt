cmake_minimum_required(VERSION 3.30)

project(SeaBattleBot)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)

# nlohmann_json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Crow
FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.0+5
)
FetchContent_MakeAvailable(crow)

# Boost
find_package(Boost 1.65 REQUIRED COMPONENTS system)

# TgBot
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/TgBot)
find_package(TgBot REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

if(NOT SOURCES)
    message(FATAL_ERROR "No .cpp files found in src/")
endif()

# Executable
add_executable(SeaBattleBot ${SOURCES})

# Link libraries
target_link_libraries(SeaBattleBot
        PRIVATE
        TgBot::TgBot
        nlohmann_json::nlohmann_json
        Crow::Crow
        Boost::system
)