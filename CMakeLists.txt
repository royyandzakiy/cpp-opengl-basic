cmake_minimum_required(VERSION 3.28)

# Machine-local overrides (e.g. VCPKG_ROOT_PATH) without touching tracked files. Optional.
include(project_options.local.cmake OPTIONAL)

#=========================
# vcpkg (manifest mode) — must run BEFORE project()
#=========================
# Manifest mode is auto-enabled because vcpkg.json sits next to this file; deps install on
# configure. We only need to point CMake at the vcpkg toolchain. Resolution order:
#   CMAKE_TOOLCHAIN_FILE (preset/-D) > VCPKG_ROOT env > VCPKG_ROOT_PATH var > common locations.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VCPKG_ROOT})
    set(_vcpkg_root "$ENV{VCPKG_ROOT}")
  elseif(DEFINED VCPKG_ROOT_PATH)
    set(_vcpkg_root "${VCPKG_ROOT_PATH}")
  else()
    foreach(_p "C:/vcpkg" "$ENV{USERPROFILE}/vcpkg" "/opt/vcpkg" "$ENV{HOME}/vcpkg" "/usr/local/vcpkg")
      if(EXISTS "${_p}/scripts/buildsystems/vcpkg.cmake")
        set(_vcpkg_root "${_p}")
        break()
      endif()
    endforeach()
  endif()
  if(_vcpkg_root AND EXISTS "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake"
        CACHE FILEPATH "vcpkg toolchain")
    message(STATUS "[vcpkg] ${_vcpkg_root}")
  else()
    message(FATAL_ERROR
      "vcpkg not found. Set the VCPKG_ROOT env var, pass -DVCPKG_ROOT_PATH=<path>, or copy "
      "CMakeUserPresets.json.example to CMakeUserPresets.json and set VCPKG_ROOT_PATH there.")
  endif()
endif()

#=========================
# Project
#=========================
project(HelloOpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23) # bump to 26 per-preset for reflection/modules experiments
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Single-config generators (Ninja): default to Debug when a preset didn't set one.
get_property(_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT _multi_config AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Light warnings only — no -Werror, nothing harsh (this is a prototyping base).
if(MSVC)
  add_compile_options(/W3 /utf-8)
else()
  add_compile_options(-Wall -Wextra)
endif()

option(BUILD_TESTING "Build the unit tests" OFF)
option(BUILD_EXAMPLES "Build the examples" OFF)

include(cmake/clangd.cmake) # mirror compile_commands.json to repo root for clangd
include(cmake/format.cmake) # `format` / `format-check` targets

#=========================
# Dependencies (vcpkg.json)
#=========================
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

#=========================
# Main executable
#=========================
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} 
  PRIVATE 
    fmt::fmt
    glfw
    OpenGL::GL
)

#=========================
# Tests & examples
#=========================
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()