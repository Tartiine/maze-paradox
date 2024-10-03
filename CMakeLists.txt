cmake_minimum_required(VERSION 3.10)
project(MazeParadox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT DEFINED ENV{FANN_DIR})
    message(FATAL_ERROR "FANN_DIR environment variable is not set.")
endif()

set(FANN_DIR $ENV{FANN_DIR})

# Add include directories for FANN and local includes
include_directories(${FANN_DIR}/src/include ./include)

# Find SFML libraries. Adjust the path if necessary.
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

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
add_executable(game ${SOURCES})

# Link the FANN library (make sure to check if the path is correct)
find_library(FANN_LIB fann HINTS ${FANN_DIR}/build/lib)

# Link the SFML and FANN libraries to the executable
target_link_libraries(game
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    ${FANN_LIB} 
)

# Copy resources directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
