cmake_minimum_required(VERSION 3.16)
project(layout-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/box_model.cpp
    src/layout_node.cpp
    src/layout_engine.cpp
    src/block_layout.cpp
    src/inline_layout.cpp
    src/flexbox_layout.cpp
    src/grid_layout.cpp
    src/positioned_layout.cpp
    src/text_layout.cpp
    src/stacking_context.cpp
    src/viewport.cpp
    src/geometry.cpp
)

# Header files
set(HEADERS
    include/layout/box_model.h
    include/layout/layout_node.h
    include/layout/layout_engine.h
    include/layout/block_layout.h
    include/layout/inline_layout.h
    include/layout/flexbox_layout.h
    include/layout/grid_layout.h
    include/layout/positioned_layout.h
    include/layout/text_layout.h
    include/layout/stacking_context.h
    include/layout/viewport.h
    include/layout/geometry.h
    include/layout/types.h
    include/layout/enums.h
)

# Create library
add_library(layout-engine STATIC ${SOURCES} ${HEADERS})

# Compiler flags
target_compile_options(layout-engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# Link libraries
find_package(Threads REQUIRED)
target_link_libraries(layout-engine Threads::Threads)

# Installation
install(TARGETS layout-engine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
