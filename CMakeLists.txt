cmake_minimum_required(VERSION 3.22)
project(Mara_nx C CXX ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# We have to add the cmake directory to have access to
# the Switch tools and also find_package for libnx.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# SwitchTools provide various helper functions for building Switch file formats.
include(SwitchTools)

# Find libnx, which is required by our application.
find_package(LIBNX REQUIRED)
add_compile_definitions(HAVE_LIBNX)

## Borealis resource location
add_compile_definitions(BOREALIS_RESOURCES="romfs:/")

set(LIBS_DIR Libs/)
set(BOREALIS_DIR ${LIBS_DIR}/borealis/)
set(XDELTA3_DIR ${LIBS_DIR}xdelta3-nx)

# Borealis source with dependencies
file(GLOB BOREALIS_SRC
        ${BOREALIS_DIR}/library/lib/*.cpp
        ${BOREALIS_DIR}/library/lib/extern/*/*.c
        ${BOREALIS_DIR}/library/lib/extern/*/*/*.c
        ${BOREALIS_DIR}/library/lib/*.cpp
        ${BOREALIS_DIR}/library/lib/*.c
        ${BOREALIS_DIR}/library/lib/extern/glad/*.c
        ${BOREALIS_DIR}/library/lib/extern/nanovg/*.c
        ${BOREALIS_DIR}/library/lib/extern/libretro-common/compat/*.c
        ${BOREALIS_DIR}/library/lib/extern/libretro-common/encodings/*.c
        ${BOREALIS_DIR}/library/lib/extern/libretro-common/features/*.c
        ${BOREALIS_DIR}/library/lib/extern/switch-libpulsar/src/*/*.c
        ${BOREALIS_DIR}/library/lib/extern/fmt/src/*.cc
        )

# Xdelta 3 source
add_subdirectory(${XDELTA3_DIR}/Sources/Library/)

## Project sources
include_directories(${PROJECT_SOURCE_DIR}/include/)
file(GLOB_RECURSE SOURCES_ASM ${PROJECT_SOURCE_DIR}/source/*.s)
file(GLOB_RECURSE SOURCES_C ${PROJECT_SOURCE_DIR}/source/*.c
        ${PROJECT_SOURCE_DIR}/source/*/*.c
        ${PROJECT_SOURCE_DIR}/source/*/*/*.c)
file(GLOB_RECURSE SOURCES_CXX ${PROJECT_SOURCE_DIR}/source/*.cpp
        ${PROJECT_SOURCE_DIR}/source/*/*.cpp
        ${PROJECT_SOURCE_DIR}/source/*/*/*.cpp)

add_executable(Mara_nx ${SOURCES_ASM} ${SOURCES_C} ${SOURCES_CXX} ${BOREALIS_SRC})
target_compile_definitions(Mara_nx PRIVATE PROJECT_NAME="${PROJECT_NAME}")
# Next, we need to set the required target properties
# so CMake can detect the metadata for the application.
set_target_properties(Mara_nx PROPERTIES
        APP_TITLE "Mara"
        APP_AUTHOR "TraduSquare"
        APP_VERSION "0.1.0"
        ROMFS "${CMAKE_CURRENT_LIST_DIR}/resources"
        )


target_include_directories(Mara_nx PUBLIC
        include/
        ${BOREALIS_DIR}/library/include
        ${BOREALIS_DIR}/library/include/borealis/extern
        ${BOREALIS_DIR}/library/lib/extern/fmt/include
        ${BOREALIS_DIR}/library/lib/extern/switch-libpulsar/include
        ${BOREALIS_DIR}/library/include
        ${XDELTA3_DIR}/Sources/Library/include
        )

# Portlibs and libnx static libraries
include_directories("${PORTLIBS}/include")
include_directories("${LIBNX}/include")

target_link_libraries(Mara_nx
        switch::libnx
        xdelta3_nx
        -L${LIBNX}/lib
        -L${PORTLIBS}/lib
        -lglfw3 -lEGL -lglad -lglapi -ldrm_nouveau -lm -lnx
        )

# NX executable target
add_nro_target(Mara_nx)