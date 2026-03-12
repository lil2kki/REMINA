cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(here)

file( DOWNLOAD "https://github.com/lil2kki/geode-download-dependency-cmake/blob/main/.cmake?raw=true" 
    ${CMAKE_CURRENT_BINARY_DIR}/geode-download-dependency.cmake)
include(${CMAKE_CURRENT_BINARY_DIR}/geode-download-dependency.cmake)

download_dependency_mod(lil2kki/geode-game-objects-factory user95401.game-objects-factory FALSE)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
include_directories(src)

# httplib idi nahui
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    WIN32_LEAN_AND_MEAN
    _WINSOCK_DEPRECATED_NO_WARNINGS
)

target_compile_options(${PROJECT_NAME} PRIVATE 
-Wno-implicit-const-int-float-conversion 
-Wno-undefined-bool-conversion
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(WARNING "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
    # git clone https://github.com/geode-sdk/geode ~/Geode-SDK
    set(ENV{GEODE_SDK} "/home/codespace/Geode-SDK")
    set(GEODE_TARGET_PLATFORM "Android64")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
