cmake_minimum_required(VERSION 3.10)
project(Aerospace VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------
# ARINC653 Library
# -----------------------------
add_library(arinc653
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Src/error.c
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Src/ipc_queueing.c
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Src/ipc_sampling.c
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Src/partition.c
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Src/process.c
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Src/scheduler.c
)
target_include_directories(arinc653 PUBLIC
    ${CMAKE_SOURCE_DIR}/RTOS/ARINC653/Inc
)

# -----------------------------
# Communication Libraries
# -----------------------------
add_library(arinc429
    ${CMAKE_SOURCE_DIR}/Comm/ARINC429/Src/arinc429.c)
target_include_directories(arinc429 PUBLIC 
    ${CMAKE_SOURCE_DIR}/Comm/ARINC429/Inc
)

add_library(arinc664 
    ${CMAKE_SOURCE_DIR}/Comm/ARINC664/Src/arinc664.c)
target_include_directories(arinc664 PUBLIC 
    ${CMAKE_SOURCE_DIR}/Comm/ARINC664/Inc
)

add_library(milstd1553b 
    ${CMAKE_SOURCE_DIR}/Comm/MILSTD1553B/Src/milstd1553b.c)
target_include_directories(milstd1553b PUBLIC 
    ${CMAKE_SOURCE_DIR}/Comm/MILSTD1553B/Inc
)

# -----------------------------
# User Partitions + Runner
# -----------------------------
file(GLOB PARTITION_SRC 
    ${CMAKE_SOURCE_DIR}/Partitions/Src/*.c)
file(GLOB RUNNER_SRC 
    ${CMAKE_SOURCE_DIR}/Runner/*.c)
add_executable(${PROJECT_NAME}
    ${PARTITION_SRC}
    ${RUNNER_SRC}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    Partitions/Inc
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    arinc653 
    arinc429 
    arinc664 
    milstd1553b
)