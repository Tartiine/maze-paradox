cmake_minimum_required(VERSION 3.20)
project(MazeParadox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the CMake prefix path to help locate cppflow
set(CMAKE_PREFIX_PATH "C:/libs/cppflow")

# Manually specify TensorFlow include and library paths
set(tensorflow_INCLUDE_DIRS "C:/libs/tensorflow/include")
set(tensorflow_LIBRARIES "C:/libs/tensorflow/lib/tensorflow.dll")

include_directories(./include ${tensorflow_INCLUDE_DIRS})

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
find_package(cppflow REQUIRED)

# Add the source files
file(GLOB SOURCES "src/*.cpp")

set(WINDOWS_RESOURCES resources/rc/resources.rc)
add_executable(MazeParadox ${SOURCES} ${WINDOWS_RESOURCES})

target_include_directories(MazeParadox PUBLIC
    cppflow::cppflow
)

# Link the SFML and TensorFlow libraries to the executable
target_link_libraries(MazeParadox PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    cppflow::cppflow
    ${tensorflow_LIBRARIES}
)

# Install the executable to the bin directory
install(TARGETS MazeParadox DESTINATION bin)

add_custom_command(
    TARGET MazeParadox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:MazeParadox>/resources
)

# Install all required DLLs explicitly
install(FILES
    "C:/msys64/mingw64/bin/libgcc_s_seh-1.dll"
    "C:/msys64/mingw64/bin/libstdc++-6.dll"
    "C:/msys64/mingw64/bin/libwinpthread-1.dll"
    "C:/msys64/mingw64/bin/libsfml-graphics-2.dll"
    "C:/msys64/mingw64/bin/libsfml-window-2.dll"
    "C:/msys64/mingw64/bin/libsfml-system-2.dll"
    "C:/msys64/mingw64/bin/libsfml-audio-2.dll"
    "C:/libs/tensorflow/lib/tensorflow.dll"
    "C:/msys64/mingw64/bin/libfreetype-6.dll"
    "C:/msys64/mingw64/bin/zlib1.dll"
    "C:/msys64/mingw64/bin/libbrotlidec.dll"
    "C:/msys64/mingw64/bin/libbrotlicommon.dll"
    "C:/msys64/mingw64/bin/libharfbuzz-0.dll"
    "C:/msys64/mingw64/bin/libpng16-16.dll"
    "C:/msys64/mingw64/bin/libintl-8.dll"
    "C:/msys64/mingw64/bin/libiconv-2.dll"
    "C:/msys64/mingw64/bin/libgraphite2.dll"
    "C:/msys64/mingw64/bin/libbz2-1.dll" 
    "C:/msys64/mingw64/bin/libglib-2.0-0.dll" 
    "C:/msys64/mingw64/bin/libpcre2-8-0.dll"
    DESTINATION bin
)

# Install the resources folder to the bin directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION bin/resources
    FILES_MATCHING 
    PATTERN "*.*"
    PATTERN "fonts/*"
    PATTERN "sprites/*"
    PATTERN "trained_model_rb/*"
)

message(STATUS "Setting CPack generator")
set(CPACK_GENERATOR "ZIP;TGZ")
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_SOURCE_DIR}/package.txt)
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_NAME "MazeParadox")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MazeParadox Game Package")

# Make sure that file build/CPackConfig.cmake exists.
# If not open MSYS type "which cpack". If the cpack wasn't found type "pacman -S mingw-w64-x86_64-cmake" then try again.
# If the issue persists try "find /mingw64 -name cpack" then "export PATH=$PATH:/mingw64/bin" to export cpack to env PATH.
# Then to package type "cpack --config build/CPackConfig.cmake", it may take a while.
include(CPack)