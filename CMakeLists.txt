cmake_minimum_required(VERSION 3.22)
project(
    kampua
    VERSION 0.0.1
    LANGUAGES C CXX
)

# Options.
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export `compile_commands.json`." ON)
set(LLVM_ROOT_DIR "/usr/local/opt/llvm" CACHE STRING "Root directory of LLVM.")

# Setup CMake.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Packages.
set(LLVM_DIR "${LLVM_ROOT_DIR}/lib/cmake/llvm")

find_package(LLVM REQUIRED)
message(STATUS "Found LLVM ${LLVM_VERSION}")

# Directories.
set(CODE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code")

# Files.
file(GLOB_RECURSE CODE_FILES "${CODE_DIR}/*.cpp")

# Targets.
add_executable(${PROJECT_NAME} ${CODE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS} ${CODE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LLVM_DEFINITIONS})
llvm_config(${PROJECT_NAME} USE_SHARED)