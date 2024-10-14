cmake_minimum_required(VERSION 3.20)
project(MazeParadox)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(./include)

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
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
    ${FANN_LIB}
)

# Copy resources directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# Install target executable
install(TARGETS MazeParadox DESTINATION bin)

# Install resources folder without nesting another resources folder
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION resources)


message(STATUS "Setting CPack generator")
set(CPACK_GENERATOR "ZIP;TGZ")
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_SOURCE_DIR}/package.txt)
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION "1.0.0")

# Make sure that file build/CPackConfig.cmake exists.
# If not open MSYS type "which cpack". If the cpack wasn't found type "pacman -S mingw-w64-x86_64-cmake" then try again.
# If the issue persists try "find /mingw64 -name cpack" then "export PATH=$PATH:/mingw64/bin" to export cpack to env PATH.
# Then to package type "cpack --config build/CPackConfig.cmake", it may take a while.
include(CPack)
