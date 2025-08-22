cmake_minimum_required(VERSION 3.14)
project(JiFFI2 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CLI_ONLY option
option(CLI_ONLY "Build CLI-only version without Qt5 and TinyXML2" OFF)

if(CLI_ONLY)
    message(STATUS "Building CLI-only version")
    add_compile_definitions(CLI_ONLY)
else()
    message(STATUS "Building full GUI version")
endif()

message(STATUS "Configuring ${PROJECT_NAME} build")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Paths
set(LOCAL_INCLUDE_HINT "/usr/local/include")
set(LOCAL_LIB_HINT "/usr/local/lib")

# Qt5 (only if not CLI_ONLY)
if(NOT CLI_ONLY)
    set(CMAKE_AUTOMOC OFF)
    set(CMAKE_AUTOUIC OFF)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    message(STATUS "Found Qt ${Qt5_VERSION}")
    message(STATUS "AUTOUIC disabled, using explicit UI processing")
    message(STATUS "Qt5Core_MOC_EXECUTABLE: ${Qt5Core_MOC_EXECUTABLE}")
endif()

# tinyxml2 library package based on CMake (only if not CLI_ONLY)
if(NOT CLI_ONLY)
    find_package(tinyxml2 REQUIRED)
endif()

# libelf library package based on autotools
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBELF REQUIRED libelf)

# MOC generation (only if not CLI_ONLY)
set(MOC_SOURCES "")
if(NOT CLI_ONLY)
    set(MOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated_moc)
    file(MAKE_DIRECTORY ${MOC_OUTPUT_DIR})

    set(MOC_HEADERS_LIST
        "src/about.h"
        "src/extraopts.h"
        "src/JiFFI2.h"
    )
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
endif()

# UI processing (only if not CLI_ONLY)
set(UI_HEADERS "")
if(NOT CLI_ONLY)
    set(UI_OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated_ui_headers)
    file(MAKE_DIRECTORY ${UI_OUTPUT_DIR})

    set(UI_FILES
        Resources/JiFFI2.ui
        Resources/about.ui
        Resources/extraopts.ui
    )

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
endif()

# Sources
set(SOURCES
    src/bjl.cpp
    src/coff.cpp
    src/common.cpp
    src/crc.cpp
    src/elf.cpp
    src/format.cpp
    src/jagr2.cpp
    src/jagr3.cpp
    src/main.cpp
    src/patches.cpp
    src/rom.cpp
)

# Add GUI-specific sources only if not CLI_ONLY
if(NOT CLI_ONLY)
    list(APPEND SOURCES
        src/about.cpp
        src/extraopts.cpp
        src/JiFFI2.cpp
    )
endif()

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

# Include directories
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIBELF_INCLUDE_DIRS}
)

if(NOT CLI_ONLY)
    list(APPEND INCLUDE_DIRS ${UI_OUTPUT_DIR} ${MOC_OUTPUT_DIR})
endif()

target_include_directories(JiFFI2 PRIVATE ${INCLUDE_DIRS})

# Link libraries
set(LINK_LIBS
    ${LIBELF_LINK_LIBRARIES}
)

if(NOT CLI_ONLY)
    list(APPEND LINK_LIBS Qt5::Widgets tinyxml2::tinyxml2)
endif()

target_link_libraries(JiFFI2 PRIVATE ${LINK_LIBS})

# Install the executable
install(TARGETS JiFFI2
    RUNTIME DESTINATION bin
)

# Project is installable
include(CMakePackageConfigHelpers)

message(STATUS "Configuration of ${PROJECT_NAME} complete")
