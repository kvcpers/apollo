cmake_minimum_required(VERSION 3.16)
project(renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/renderer.cpp
    src/canvas.cpp
    src/paint.cpp
    src/path.cpp
    src/image.cpp
    src/font.cpp
    src/text.cpp
    src/shader.cpp
    src/effects.cpp
    src/compositor.cpp
    src/layer.cpp
    src/transform.cpp
    src/clip.cpp
    src/blend.cpp
    src/filter.cpp
    src/gradient.cpp
    src/pattern.cpp
    src/surface.cpp
    src/context.cpp
    src/device.cpp
    src/backend.cpp
    src/gpu.cpp
    src/software.cpp
    src/hardware.cpp
)

# Header files
set(HEADERS
    include/renderer/types.h
    include/renderer/enums.h
    include/renderer/color.h
    include/renderer/point.h
    include/renderer/rect.h
    include/renderer/matrix.h
    include/renderer/paint.h
    include/renderer/path.h
    include/renderer/image.h
    include/renderer/font.h
    include/renderer/text.h
    include/renderer/shader.h
    include/renderer/effects.h
    include/renderer/compositor.h
    include/renderer/layer.h
    include/renderer/transform.h
    include/renderer/clip.h
    include/renderer/blend.h
    include/renderer/filter.h
    include/renderer/gradient.h
    include/renderer/pattern.h
    include/renderer/surface.h
    include/renderer/context.h
    include/renderer/device.h
    include/renderer/backend.h
    include/renderer/gpu.h
    include/renderer/software.h
    include/renderer/hardware.h
    include/renderer/canvas.h
    include/renderer/renderer.h
)

# Create library
add_library(renderer STATIC ${SOURCES} ${HEADERS})

# Compiler flags
target_compile_options(renderer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# Link libraries
find_package(Threads REQUIRED)
target_link_libraries(renderer Threads::Threads)

# Installation
install(TARGETS renderer
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
