cmake_minimum_required(VERSION 3.16)

project(LoggerFramework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Threads Support ----
find_package(Threads REQUIRED)

# ---- Library Definition ----
add_library(LoggerLib STATIC
    src/Logger.cpp
    src/ConsoleHandler.cpp
    src/FileHandler.cpp
    src/DefaultFormatter.cpp
    src/CsvFormatter.cpp
    src/JsonFormatter.cpp
    src/Formatter.cpp
    src/Handler.cpp
    src/LogRotator.cpp
    src/MemoryHandler.cpp
    src/CrashGuard.cpp
    src/HandlerFactory.cpp
    src/ConfigManager.cpp
)

# PUBLIC: Users of this library will automatically get the include directory
target_include_directories(LoggerLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link threads to the library itself
target_link_libraries(LoggerLib PUBLIC Threads::Threads)

# ---- Examples (Executables) ----

# 1. Basic Usage Example
add_executable(BasicUsage samples/BasicUsage.cpp)
target_link_libraries(BasicUsage PRIVATE LoggerLib)

# 2. Hot Reload Demo
add_executable(HotReloadDemo samples/HotReloadDemo.cpp)
target_link_libraries(HotReloadDemo PRIVATE LoggerLib)

# 3. Incident Reporting (Memory Handler) Demo
add_executable(IncidentReporting samples/IncidentReporting.cpp)
target_link_libraries(IncidentReporting PRIVATE LoggerLib)

# ---- Compiler Warnings ----
if (MSVC)
    target_compile_options(LoggerLib PRIVATE /W4)
else()
    target_compile_options(LoggerLib PRIVATE -Wall -Wextra -Wpedantic)
endif()