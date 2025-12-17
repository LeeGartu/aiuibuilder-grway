cmake_minimum_required(VERSION 3.10)
project(p)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(rt-thread)

set(FLASHDB FlashDB-1.1.2)
file(GLOB_RECURSE FDB_SOURCES  "${FLASHDB}/src/*.c")
file(GLOB_RECURSE BSP_SOURCES  "bsp/src/*.c")
file(GLOB_RECURSE MACH_SOURCES  "mach/src/*.c")

include_directories(${PROJECT_SOURCE_DIR}/${FLASHDB}/inc/)
include_directories(${PROJECT_SOURCE_DIR}/bsp/inc/)
include_directories(${PROJECT_SOURCE_DIR}/mach/inc/)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_CXX_FLAGS "-O3")

add_library(p STATIC
    ${FDB_SOURCES}
    ${BSP_SOURCES}
    ${MACH_SOURCES}
)

target_include_directories(p PUBLIC 
    ${PROJECT_SOURCE_DIR}/${FLASHDB}/inc/
    ${PROJECT_SOURCE_DIR}/bsp/inc/
    ${PROJECT_SOURCE_DIR}/mach/inc/
)

target_link_libraries(p PUBLIC rtthread)
target_link_libraries(p PRIVATE Shlwapi)
target_link_libraries(p PRIVATE libserialport)
