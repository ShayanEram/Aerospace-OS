cmake_minimum_required(VERSION 3.10)
project(AeroRTOS VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------
# ARINC653 Library
# -----------------------------
add_library(arinc653
    RTOS/ARINC653/Src/error.c
    RTOS/ARINC653/Src/ipc_queueing.c
    RTOS/ARINC653/Src/ipc_sampling.c
    RTOS/ARINC653/Src/partition.c
    RTOS/ARINC653/Src/process.c
    RTOS/ARINC653/Src/scheduler.c
)
target_include_directories(arinc653 PUBLIC
    RTOS/ARINC653/Inc
)

# -----------------------------
# Communication Libraries
# -----------------------------
add_library(arinc429
    Comm/ARINC429/Src/arinc429.c)
target_include_directories(arinc429 PUBLIC 
    Comm/ARINC429/Inc
)

add_library(arinc664 
    Comm/ARINC664/Src/arinc664.c)
target_include_directories(arinc664 PUBLIC 
    Comm/ARINC664/Inc
)

add_library(milstd1553b 
    Comm/MILSTD1553B/Src/milstd1553b.c)
target_include_directories(milstd1553b PUBLIC 
    Comm/MILSTD1553B/Inc
)

# -----------------------------
# User Partitions + Runner
# -----------------------------
file(GLOB PARTITION_SRC 
    Partitions/Partition1/Process1/Src/*.c
    Partitions/Partition2/Process1/Src/*.c
)
file(GLOB RUNNER_SRC 
    Runner/*.c
)
add_executable(${PROJECT_NAME}
    ${PARTITION_SRC}
    ${RUNNER_SRC}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    Partitions/Partition1/Process1/Inc
    Partitions/Partition2/Process1/Inc
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    arinc653 
    arinc429 
    arinc664 
    milstd1553b
)