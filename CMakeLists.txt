cmake_minimum_required(VERSION 3.23)
project(cmake_switch_template)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
enable_language(ASM C CXX)

# We have to add the cmake directory to have access to
# the Switch tools and also find_package for libnx.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# SwitchTools provide various helper functions for building Switch file formats.
include(SwitchTools)

# Find libnx, which is required by our application.
find_package(LIBNX REQUIRED)

## Project sources
include_directories(${PROJECT_SOURCE_DIR}/include/)
file(GLOB_RECURSE SOURCES_ASM ${PROJECT_SOURCE_DIR}/source/*.s)
file(GLOB_RECURSE SOURCES_C ${PROJECT_SOURCE_DIR}/source/*.c)
file(GLOB_RECURSE SOURCES_CXX ${PROJECT_SOURCE_DIR}/source/*.cpp)

add_executable(cmake_switch_template ${SOURCES_ASM} ${SOURCES_C} ${SOURCES_CXX})

# Next, we need to set the required target properties
# so CMake can detect the metadata for the application.
set_target_properties(cmake_switch_template PROPERTIES
        APP_TITLE "Template Application"
        APP_AUTHOR "Valentin B."
        APP_VERSION "0.1.0"
        ROMFS "${CMAKE_CURRENT_LIST_DIR}/romfs")

# Then, we link the required libraries (libnx) against the executable.
target_link_libraries(cmake_switch_template switch::libnx)

# Set these variables if you don't want to use an icon or
# don't want `add_nro_target` to generate a .nacp file.
set(NO_ICON TRUE)
#set(NO_NACP TRUE)

# And finally we can add the actual binary (.nro) target.
add_nro_target(cmake_switch_template)
