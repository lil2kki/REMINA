cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(here)

# geode-game-objects-factory

file(DOWNLOAD "https://github.com/lil2kki/geode-download-dependency-cmake/blob/main/.cmake?raw=true" 
    "${CMAKE_CURRENT_BINARY_DIR}/geode-download-dependency.cmake")
include("${CMAKE_CURRENT_BINARY_DIR}/geode-download-dependency.cmake")
download_dependency_mod("lil2kki/geode-game-objects-factory" "user95401.game-objects-factory" FALSE)

# SOURCES

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

set(MLC_CPP "${CMAKE_CURRENT_BINARY_DIR}/Main-Levels-Config.cpp")
file(DOWNLOAD "https://raw.githubusercontent.com/lil2kki/Main-Levels-Config/HEAD/src/main.cpp" ${MLC_CPP})

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${MLC_CPP})
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

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

# for Main-Levels-Config
CPMAddPackage(
    NAME simpleini
    GITHUB_REPOSITORY brofield/simpleini
    GIT_TAG origin/master
)
include_directories(${simpleini_SOURCE_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC EXCLUDE_MLC_INGAME_CONFIG_SUPPORT)