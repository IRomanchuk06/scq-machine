cmake_minimum_required(VERSION 3.11)
project(ostis-example-app)

# Set path to sc-machine project
set(SC_MACHINE_PATH "${CMAKE_CURRENT_LIST_DIR}/ostis-web-platform/sc-machine")
add_subdirectory("${SC_MACHINE_PATH}")

# Set path to scq-machine project
set(SCQ_MACHINE_PATH "${CMAKE_CURRENT_LIST_DIR}/scq-machine")
add_subdirectory(${SCQ_MACHINE_PATH})

# Set output directories for binaries and libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin/extensions")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

# For multi-config builds (e.g., MSVC)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
endforeach()

# Add problem-solver module
add_subdirectory(problem-solver)

# Macro to list subdirectories within a directory
macro(subdir_list result current_dir)
    file(GLOB children RELATIVE ${current_dir} ${current_dir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${current_dir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

# Include each subsystem from the subsystems directory
set(SUBSYSTEMS_PATH "${CMAKE_CURRENT_LIST_DIR}/subsystems")
subdir_list(SUBDIRS ${SUBSYSTEMS_PATH})

foreach(SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBSYSTEMS_PATH}/${SUBDIR}/problem-solver)
endforeach()
