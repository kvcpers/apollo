cmake_minimum_required(VERSION 3.20)
project(BrowserEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

# Skia for rendering
find_library(SKIA_LIB NAMES skia PATHS /usr/local/lib /opt/homebrew/lib)
if(SKIA_LIB)
    message(STATUS "Found Skia: ${SKIA_LIB}")
else()
    message(WARNING "Skia not found, rendering will be limited")
endif()

# Cairo alternative
pkg_check_modules(CAIRO REQUIRED)

# Platform-specific libraries
if(WIN32)
    set(PLATFORM_LIBS user32 gdi32 shell32 ole32)
elseif(APPLE)
    find_library(COREFOUNDATION CoreFoundation)
    find_library(COCOA Cocoa)
    find_library(SECURITY Security)
    set(PLATFORM_LIBS ${COREFOUNDATION} ${COCOA} ${SECURITY})
elseif(UNIX)
    set(PLATFORM_LIBS X11)
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/parser
    ${CMAKE_SOURCE_DIR}/layout
    ${CMAKE_SOURCE_DIR}/network
    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_SOURCE_DIR}/storage
    ${CMAKE_SOURCE_DIR}/tests
)

# Core browser components
add_subdirectory(core/browser)
add_subdirectory(parser/html)
add_subdirectory(parser/css)
add_subdirectory(layout)
add_subdirectory(network)
add_subdirectory(ui)
add_subdirectory(storage)

# Main browser executable
add_executable(browser_engine
    core/main.cpp
)

target_link_libraries(browser_engine
    core_browser
    parser_html
    parser_css
    layout_engine
    network_stack
    ui_chrome
    storage_manager
    ${CAIRO_LIBRARIES}
    ${PLATFORM_LIBS}
    Threads::Threads
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(browser_engine PRIVATE
        -Wall -Wextra -Wpedantic
        -O3 -flto -march=native
    )
endif()

# Installation
install(TARGETS browser_engine DESTINATION bin)

# Testing
enable_testing()
add_subdirectory(tests)
