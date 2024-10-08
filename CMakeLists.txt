cmake_minimum_required(VERSION 3.20)
project(MazeParadox)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(./include)


find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
find_library(FANN_LIB NAMES fann HINTS /mingw64/lib /mingw32/lib)

# Add the source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Animation.cpp
    src/Platform.cpp
    src/Collision.cpp
    src/Obstacle.cpp
    src/Ground.cpp
    src/TileMap.cpp
    src/TileMapGenerator.cpp
    src/RuleBasedGenerator.cpp
    src/NoiseBasedGenerator.cpp
    src/TileMapModel.cpp
    src/TileMapManager.cpp
)

# Create the executable
add_executable(MazeParadox ${SOURCES})

# Link the SFML and FANN libraries to the executable
target_link_libraries(MazeParadox
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    ${FANN_LIB}  # Use the found fann library
)

# Copy resources directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})