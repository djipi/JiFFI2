cmake_minimum_required(VERSION 3.14)
project(JiFFI2 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Configuring ${PROJECT_NAME} build")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Paths
set(LOCAL_INCLUDE_HINT "/usr/local/include")
set(LOCAL_LIB_HINT "/usr/local/lib")

# libcrc32.a (static library manually installed)
find_path(LIBCRC32_LIB_DIR 
    NAMES libcrc32.a
    PATHS ${LOCAL_LIB_HINT}
    DOC "Path to libcrc32.a library file"
)
if(LIBCRC32_LIB_DIR)
    message(STATUS "Found libcrc32.a in: ${LIBCRC32_LIB_DIR}")
else()
    message(FATAL_ERROR "libcrc32.a not found! Please check installation.")
endif()

find_path(LIBCRC32_INCLUDE_DIR 
    NAMES crc32.h
    PATHS ${LOCAL_INCLUDE_HINT}
    DOC "Path to crc32.h header file"
)
if(LIBCRC32_INCLUDE_DIR)
    message(STATUS "Found crc32.h in: ${LIBCRC32_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "crc32.h not found! Please check installation.")
endif()

add_library(libcrc32 STATIC IMPORTED GLOBAL)
set_target_properties(libcrc32 PROPERTIES
    IMPORTED_LOCATION "${LIBCRC32_LIB_DIR}/libcrc32.a"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBCRC32_INCLUDE_DIR}"
)
message(STATUS "Created imported target for libcrc32")

# libcoff.h (header-only, manually installed)
find_path(LIBCOFF_INCLUDE_DIR 
    NAMES libcoff.h
    PATHS ${LOCAL_INCLUDE_HINT}
    DOC "Path to libcoff.h header file"
)
if(LIBCOFF_INCLUDE_DIR)
    message(STATUS "Found libcoff.h in: ${LIBCOFF_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "libcoff.h not found! Please check installation.")
endif()

add_library(libcoff INTERFACE)
target_include_directories(libcoff INTERFACE "${LIBCOFF_INCLUDE_DIR}")
message(STATUS "Created interface target for libcoff")

# Qt5
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

message(STATUS "Found Qt ${Qt5_VERSION}")
message(STATUS "AUTOUIC disabled, using explicit UI processing")
message(STATUS "Qt5Core_MOC_EXECUTABLE: ${Qt5Core_MOC_EXECUTABLE}")

# tinyxml2 library package based on CMake
find_package(tinyxml2 REQUIRED)
# libelf library package based on autotools
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBELF REQUIRED libelf)

# MOC generation
set(MOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated_moc)
file(MAKE_DIRECTORY ${MOC_OUTPUT_DIR})

set(MOC_HEADERS_LIST
    "src/about.h"
    "src/extraopts.h"
    "src/JiFFI2.h"
)
set(MOC_SOURCES "")
foreach(header_file ${MOC_HEADERS_LIST})
    set(header_absolute "${CMAKE_CURRENT_SOURCE_DIR}/${header_file}")
    get_filename_component(header_name ${header_file} NAME_WE)
    set(moc_output "${MOC_OUTPUT_DIR}/moc_${header_name}.cpp")

    if(EXISTS ${header_absolute})
        message(STATUS "${header_file} found at: ${header_absolute}")
        add_custom_command(
            OUTPUT ${moc_output}
            COMMAND ${Qt5Core_MOC_EXECUTABLE} -o ${moc_output} ${header_absolute}
            MAIN_DEPENDENCY ${header_absolute}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating MOC for ${header_file}"
            VERBATIM
        )
        list(APPEND MOC_SOURCES ${moc_output})
    else()
        message(FATAL_ERROR "${header_file} does not exist at: ${header_absolute}")
    endif()
endforeach()
message(STATUS "MOC files to be generated: ${MOC_SOURCES}")

# UI processing
set(UI_OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated_ui_headers)
file(MAKE_DIRECTORY ${UI_OUTPUT_DIR})

set(UI_FILES
    Resources/JiFFI2.ui
    Resources/about.ui
    Resources/extraopts.ui
)

set(UI_HEADERS "")
foreach(ui_file ${UI_FILES})
    get_filename_component(ui_name ${ui_file} NAME_WE)
    set(ui_header "${UI_OUTPUT_DIR}/ui_${ui_name}.h")
    message(STATUS "Will generate: ${ui_header}")
    add_custom_command(
        OUTPUT ${ui_header}
        COMMAND ${Qt5Widgets_UIC_EXECUTABLE} -o ${ui_header} ${CMAKE_CURRENT_SOURCE_DIR}/${ui_file}
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${ui_file}
        COMMENT "Generating ${ui_header}"
    )
    list(APPEND UI_HEADERS ${ui_header})
endforeach()

# Sources
set(SOURCES
    src/about.cpp
    src/bjl.cpp
    src/coff.cpp
    src/common.cpp
    src/crc.cpp
    src/elf.cpp
    src/extraopts.cpp
    src/format.cpp
    src/jagr2.cpp
    src/jagr3.cpp
    src/JiFFI2.cpp
    src/main.cpp
    src/patches.cpp
    src/rom.cpp
)

set(RESOURCES
    Resources/jagrhead.cpp
    Resources/elfstringtable.cpp
    Resources/elfhead.cpp
    Resources/pad.cpp
    Resources/pad2.cpp
    Resources/romhead2.cpp
)

set(HEADERS
    src/about.h
    src/bjl.h
    src/coff.h
    src/common.h
    src/crc.h
    src/elf.h
    src/extraopts.h
    src/format.h
    src/jagr2.h
    src/jagr3.h
    src/JiFFI2.h
    src/patches.h
    src/rom.h
    src/version.h
)

# Target
add_executable(JiFFI2 ${SOURCES} ${RESOURCES} ${MOC_SOURCES} ${UI_HEADERS})
message(STATUS "Created executable target: JiFFI2")

target_include_directories(JiFFI2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${UI_OUTPUT_DIR}
    ${MOC_OUTPUT_DIR}
    ${LIBELF_INCLUDE_DIRS}
)

target_link_libraries(JiFFI2 PRIVATE
    Qt5::Widgets
    tinyxml2::tinyxml2
    ${LIBELF_LINK_LIBRARIES}
    libcrc32
    libcoff
)

# Install the executable
install(TARGETS JiFFI2
    RUNTIME DESTINATION bin
)

# Project is installable
include(CMakePackageConfigHelpers)

message(STATUS "Configuration of ${PROJECT_NAME} complete")
