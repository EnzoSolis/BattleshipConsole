cmake_minimum_required(VERSION 3.24)
project(Battleship C)

##########################
# Global Compiler Options
##########################
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

if (
  CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
  (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
)
  set(CMAKE_C_FLAGS         "-Wall -Wextra")
  set(CMAKE_C_FLAGS_DEBUG   "-O0 -g -DDEBUG -D_DEBUG")
  set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

elseif (
  CMAKE_C_COMPILER_ID STREQUAL "MSVC" OR
  (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
) 
  set(CMAKE_C_FLAGS         "/W4")
  set(CMAKE_C_FLAGS_DEBUG   "/Od /Zi /DDEBUG /D_DEBUG")
  set(CMAKE_C_FLAGS_RELEASE "/O3 /DNDEBUG")

else()
  message(WARNING "hasn't set compiler flags for ${CMAKE_C_COMPILER_ID} compiler.")
endif()

##########################
# Binaries Output
##########################
set(BINARY_DIR "${CMAKE_CURRENT_LIST_DIR}/bin" CACHE STRING "Output directory")
set(TEST_OUTPUT_DIR "${BINARY_DIR}/tests" CACHE STRING "Output directory of tests")
set(CMAKE_BINARY_DIR ${BINARY_DIR})

foreach(config DEBUG RELEASE)
  string(TOUPPER ${config} cfg_upper)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${BINARY_DIR}/${cfg_upper}/")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${BINARY_DIR}/${cfg_upper}/")
endforeach()

##########################
# Game Sources
##########################
file(GLOB_RECURSE BS_SRCS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

##########################
# Executable Game
##########################
add_executable(${PROJECT_NAME} "${BS_SRCS}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")

#target_link_libraries(${PROJECT_NAME} PRIVATE )