cmake_minimum_required(VERSION 3.5)

project(dwwtool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


enable_language(C)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/SLADE/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/SLADE/cmake/find_modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_subdirectory(libs/SLADE/src/)

file(GLOB_RECURSE SOURCES

    #DWWTool
    src/*.cpp

    #SLADE
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Application/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Archive/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Archive/Formats/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Audio/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Game/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/General/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Graphics/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/MainEditor/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/MapEditor/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/OpenGL/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/SLADEMap/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/TextEditor/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/UI/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Utility/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS

        #DWWTool
        src/*.h

        #SLADE
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Application/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Archive/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Archive/Formats/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Audio/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Game/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/General/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Graphics/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/MainEditor/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/MapEditor/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/OpenGL/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/SLADEMap/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/TextEditor/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/UI/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/src/Utility/*.h
        ${PROJECT_SOURCE_DIR}/libs/SLADE/thirdparty/sigslot/signal.hpp

)

set(INCLUDE_DIRS "")
foreach(header ${HEADERS})
    get_filename_component(header_path "${header}" DIRECTORY)
    list(APPEND INCLUDE_DIRS "${header_path}")
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)

add_executable(dwwtool ${SOURCES} ${HEADERS})

target_link_libraries(dwwtool Qt5::Widgets)
target_link_libraries(dwwtool SLADE)
target_link_libraries(dwwtool ${wxWidgets_LIBRARIES})
target_include_directories(dwwtool PRIVATE ${INCLUDE_DIRS})
target_include_directories(dwwtool PRIVATE ${wxWidgets_INCLUDE_DIRS})

include(GNUInstallDirs)
install(TARGETS dwwtool
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
