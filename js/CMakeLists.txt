cmake_minimum_required(VERSION 3.16)
project(javascript-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/tokenizer.cpp
    src/parser.cpp
    src/ast.cpp
    src/interpreter.cpp
    src/context.cpp
    src/value.cpp
    src/object.cpp
    src/function.cpp
    src/array.cpp
    src/string.cpp
    src/number.cpp
    src/boolean.cpp
    src/null.cpp
    src/undefined.cpp
    src/error.cpp
    src/exception.cpp
    src/scope.cpp
    src/environment.cpp
    src/gc.cpp
    src/dom_bindings.cpp
    src/console.cpp
    src/event_loop.cpp
    src/promise.cpp
    src/async.cpp
    src/module.cpp
    src/loader.cpp
    src/compiler.cpp
    src/optimizer.cpp
    src/debugger.cpp
    src/profiler.cpp
    src/engine.cpp
)

# Header files
set(HEADERS
    include/js/tokenizer.h
    include/js/parser.h
    include/js/ast.h
    include/js/interpreter.h
    include/js/context.h
    include/js/value.h
    include/js/object.h
    include/js/function.h
    include/js/array.h
    include/js/string.h
    include/js/number.h
    include/js/boolean.h
    include/js/null.h
    include/js/undefined.h
    include/js/error.h
    include/js/exception.h
    include/js/scope.h
    include/js/environment.h
    include/js/gc.h
    include/js/dom_bindings.h
    include/js/console.h
    include/js/event_loop.h
    include/js/promise.h
    include/js/async.h
    include/js/module.h
    include/js/loader.h
    include/js/compiler.h
    include/js/optimizer.h
    include/js/debugger.h
    include/js/engine.h
    include/js/types.h
    include/js/enums.h
    include/js/constants.h
    include/js/utils.h
)

# Create library
add_library(javascript-engine STATIC ${SOURCES} ${HEADERS})

# Compiler flags
target_compile_options(javascript-engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# Link libraries
find_package(Threads REQUIRED)
target_link_libraries(javascript-engine Threads::Threads)

# Installation
install(TARGETS javascript-engine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
