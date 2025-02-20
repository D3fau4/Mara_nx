project(hbl C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
enable_language(ASM C CXX)

file(GLOB_RECURSE SOURCES_ASM ${PROJECT_SOURCE_DIR}/src/*.s)
file(GLOB_RECURSE SOURCES_C ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE SOURCES_CXX ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(hbl ${SOURCES_ASM} ${SOURCES_C} ${SOURCES_CXX})

set_target_properties(hbl PROPERTIES
        CONFIG_JSON ${PROJECT_SOURCE_DIR}/Application.json)

# Then, we link the required libraries (libnx) against the executable.
target_link_libraries(hbl switch::libnx)

# And finally we can add the actual binary (.nsp) target.
add_nsp_target(hbl)

add_custom_command(TARGET hbl_nsp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hbl>.nsp ${CMAKE_SOURCE_DIR}/resources/Patch
)