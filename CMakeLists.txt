cmake_minimum_required(VERSION 3.10)

# 1. Project Identity
project(MemConveyorSupervisor VERSION 1.0 LANGUAGES CXX)

# 2. Force C++17 Standard (Matches SalixGameStudio)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Include Directories
# This tells the compiler where to find "supervisor.h"
include_directories(src)

# 4. Define the Executable and its source files
# We list both .cpp files so they are linked together
add_executable(MemConveyor
    src/main.cpp
    src/supervisor.cpp
)

# 5. Visual Anchor: Output location
# This puts your .exe in the root folder so it's easy to find
set_target_properties(MemConveyor PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.."
)