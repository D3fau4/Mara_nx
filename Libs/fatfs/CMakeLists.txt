project(fatfs C)

# We have to add the cmake directory to have access to
# the Switch tools and also find_package for libnx.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# SwitchTools provide various helper functions for building Switch file formats.
include(SwitchTools)

add_library(fatfs
        source/diskio.c
        source/ff.c
        source/ffunicode.c
)

# Añade el directorio source para los includes
target_include_directories(fatfs
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(fatfs
        switch::libnx
        -L${LIBNX}/lib
        -L${PORTLIBS}/lib
)

install(TARGETS fatfs)
