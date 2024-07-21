cmake_minimum_required(VERSION 3.5)

project(dwwtool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(boost_filesystem REQUIRED)

add_subdirectory(libs/doomlib)

file(GLOB_RECURSE DWW_SOURCES
    src/*.cpp
    libs/doomlib/common.cpp
    libs/doomlib/doomlib.cpp
    libs/doomlib/thing.cpp
    libs/doomlib/wadfile.cpp
    libs/doomlib/doommap.cpp
)

file(GLOB_RECURSE DWW_HEADERS CONFIGURE_DEPENDS
    src/*.h
    libs/doomlib/common.h
    libs/doomlib/doomlib.h
    libs/doomlib/thing.h
    libs/doomlib/wadfile.h
    libs/doomlib/doommap.h
)

set(DWW_INCLUDE_DIRS "")
foreach(header ${DWW_HEADERS})
    get_filename_component(header_path "${header}" DIRECTORY)
    list(APPEND DWW_INCLUDE_DIRS "${header_path}")
endforeach()

list(REMOVE_DUPLICATES DWW_INCLUDE_DIRS)

add_executable(dwwtool ${DWW_SOURCES} ${DWW_HEADERS}
    libs/doomlib/CMakeLists.txt
    src/globals.h



)

target_include_directories(dwwtool PRIVATE ${DWW_INCLUDE_DIRS})
target_link_libraries(dwwtool Qt5::Widgets doomlib boost_filesystem)

include(GNUInstallDirs)
install(TARGETS dwwtool
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
