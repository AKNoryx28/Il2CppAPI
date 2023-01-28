cmake_minimum_required(VERSION 3.18.1)
project("Il2CppAPI")

set(DOBBY_DIR ${CMAKE_SOURCE_DIR}/Dobby)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Oz -w -s -Wall")

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Obfuscate
    ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include)

file(GLOB SRC_UNITY_TYPES src/Unity/*.cpp)

add_library(Il2CppAPI STATIC
    src/Il2CppAPI.cpp
    src/UnityEngine.cpp
    ${SRC_UNITY_TYPES}
    )

option(DOBBY_GENERATE_SHARED "Build shared library" OFF)
option(DOBBY_DEBUG "Enable debug logging" OFF)
option(NearBranch "Enable near branch trampoline" ON)
option(FullFloatingPointRegisterPack "Save and pack all floating-point registers" OFF)
option(Plugin.SymbolResolver "Enable symbol resolver" ON)
option(Plugin.ImportTableReplace "Enable import table replace " OFF)
option(Plugin.Android.BionicLinkerUtil "Enable android bionic linker util" ON)
option(BUILD_EXAMPLE "Build example" OFF)
option(BUILD_TEST "Build test" OFF)

if(NOT TARGET dobby)
macro(SET_OPTION option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
SET_OPTION(DOBBY_DEBUG OFF)
SET_OPTION(DOBBY_GENERATE_SHARED OFF)
add_subdirectory(${DOBBY_DIR} dobby)
get_property(DOBBY_INCLUDE_DIRECTORIES
  TARGET dobby
  PROPERTY INCLUDE_DIRECTORIES)
include_directories(
  .
  ${DOBBY_INCLUDE_DIRECTORIES}
  $<TARGET_PROPERTY:dobby,INCLUDE_DIRECTORIES>
)
endif()

target_link_libraries(
    Il2CppAPI
    dobby
    log
    android
    )
