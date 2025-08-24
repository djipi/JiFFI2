cmake_minimum_required(VERSION 3.14)
project(JiFFI2)
message(STATUS "Configuring ${PROJECT_NAME} build")

# Set the Makefile verbose for debugging
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Set the default build type if none can be determined
if(NOT CMAKE_CONFIGURATION_TYPES)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
else()
    message(STATUS "CMAKE_BUILD_TYPE is set to: ${CMAKE_BUILD_TYPE}")
endif()
else()
    message(STATUS "CMAKE_CONFIGURATION_TYPES is set to: ${CMAKE_CONFIGURATION_TYPES}")
endif()

# Enable C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Set the GUI build type by default
option(CLI_ONLY "Build CLI-only version without Qt5 and TinyXML2" OFF)
if(CLI_ONLY)
    message(STATUS "Building CLI-only version")
    add_compile_definitions(CLI_ONLY)
else()
    message(STATUS "Building full GUI version")
endif()

# Qt5 (only if CLI_ONLY define is off)
if(NOT CLI_ONLY)
    # Check if we're on Windows with MSVC (Visual Studio environment)
    if(WIN32 AND MSVC)
        message(STATUS "Windows/MSVC detected - checking for explicit Qt5_DIR")
        
        # Check for Qt5_DIR - first environment variable, then CMake variable
        set(QT5_ROOT_DIR "")
        if(DEFINED ENV{Qt5_DIR} AND NOT "$ENV{Qt5_DIR}" STREQUAL "")
            set(QT5_ROOT_DIR "$ENV{Qt5_DIR}")
            message(STATUS "Using Qt5_DIR from environment: ${QT5_ROOT_DIR}")
        elseif(DEFINED Qt5_DIR AND NOT Qt5_DIR STREQUAL "")
            set(QT5_ROOT_DIR "${Qt5_DIR}")
            message(STATUS "Using Qt5_DIR from CMake variable: ${QT5_ROOT_DIR}")
        endif()
        
        # If Qt5_DIR is specified, validate and use it explicitly
        if(NOT QT5_ROOT_DIR STREQUAL "")
            # Check if the provided path is the Qt5 root directory or cmake directory
            set(QT5_CMAKE_DIR "")
            
            if(EXISTS "${QT5_ROOT_DIR}/Qt5Config.cmake")
                # Direct path to cmake directory
                set(QT5_CMAKE_DIR "${QT5_ROOT_DIR}")
                message(STATUS "Qt5_DIR points to cmake directory: ${QT5_CMAKE_DIR}")
            elseif(EXISTS "${QT5_ROOT_DIR}/lib/cmake/Qt5/Qt5Config.cmake")
                # Path to Qt5 installation root, construct cmake path
                set(QT5_CMAKE_DIR "${QT5_ROOT_DIR}/lib/cmake/Qt5")
                message(STATUS "Qt5_DIR points to installation root, using cmake dir: ${QT5_CMAKE_DIR}")
            else()
                    message(FATAL_ERROR 
                        "Qt5Config.cmake not found in Qt5_DIR: ${QT5_ROOT_DIR}\n"
                        "Please ensure Qt5_DIR points to either:\n"
                        "  - Qt5 installation root (e.g., C:/Qt/5.12.0/msvc2017_64)\n"
                        "  - Qt5 cmake directory (e.g., C:/Qt/5.12.0/msvc2017_64/lib/cmake/Qt5)\n\n"
                    )
            endif()
            
            # Validate that the cmake directory exists
            if(NOT EXISTS "${QT5_CMAKE_DIR}")
                message(FATAL_ERROR 
                    "Qt5 cmake directory does not exist: ${QT5_CMAKE_DIR}\n"
                    "Please verify the path and try again.")
            endif()
            
            message(STATUS "Using Qt5 cmake directory: ${QT5_CMAKE_DIR}")
            
            # Clear CMAKE_PREFIX_PATH to ensure we only use the specified Qt5_DIR
            set(CMAKE_PREFIX_PATH_BACKUP ${CMAKE_PREFIX_PATH})
            set(CMAKE_PREFIX_PATH "")
            
            set(CMAKE_AUTOMOC OFF)
            set(CMAKE_AUTOUIC OFF)
            find_package(Qt5 COMPONENTS Widgets REQUIRED PATHS ${QT5_CMAKE_DIR} NO_DEFAULT_PATH)
            
            # Restore CMAKE_PREFIX_PATH for other packages
            set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH_BACKUP})
        else()
            # For Windows/MSVC without explicit Qt5_DIR, try automatic detection
            message(STATUS "No explicit Qt5_DIR specified - attempting automatic detection")
            message(STATUS "Note: For reliable builds on Windows, consider setting Qt5_DIR")
            message(STATUS "Example: set Qt5_DIR=C:/Qt/5.15.2/msvc2019_64")
            message(STATUS "   or:   set Qt5_DIR=C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")
            
            set(CMAKE_AUTOMOC OFF)
            set(CMAKE_AUTOUIC OFF)
            find_package(Qt5 COMPONENTS Widgets REQUIRED)
        endif()
    else()
        # For non-Windows or non-MSVC builds, use standard Qt5 detection
        message(STATUS "Non-Windows/MSVC build - using automatic Qt5 detection")
        set(CMAKE_AUTOMOC OFF)
        set(CMAKE_AUTOUIC OFF)
        find_package(Qt5 COMPONENTS Widgets REQUIRED)
    endif()

    message(STATUS "Found Qt ${Qt5_VERSION}")
    message(STATUS "AUTOUIC disabled, using explicit UI processing")
    message(STATUS "Qt5Core_MOC_EXECUTABLE: ${Qt5Core_MOC_EXECUTABLE}")
endif()

# tinyxml2 (only if not CLI_ONLY)
if(NOT CLI_ONLY)
    # Platform-specific TinyXML2 detection
    if(WIN32 AND MSVC)
        message(STATUS "Windows/MSVC detected - checking for TINYXMLDIR")
        
        # Check for TINYXMLDIR - first environment variable, then CMake variable
        # This is only available on Windows/Visual Studio
        set(TINYXML_ROOT_DIR "")
        if(DEFINED ENV{TINYXMLDIR} AND NOT "$ENV{TINYXMLDIR}" STREQUAL "")
            set(TINYXML_ROOT_DIR "$ENV{TINYXMLDIR}")
            message(STATUS "Using TINYXMLDIR from environment: ${TINYXML_ROOT_DIR}")
        elseif(DEFINED TINYXMLDIR AND NOT TINYXMLDIR STREQUAL "")
            set(TINYXML_ROOT_DIR "${TINYXMLDIR}")
            message(STATUS "Using TINYXMLDIR from CMake variable: ${TINYXML_ROOT_DIR}")
        endif()
        
        # If TINYXML_ROOT_DIR is set, use manual detection
        if(NOT TINYXML_ROOT_DIR STREQUAL "")
            message(STATUS "TINYXMLDIR specified - using manual detection")
            
            # Validate that TINYXML_ROOT_DIR exists
            if(NOT EXISTS "${TINYXML_ROOT_DIR}")
                message(FATAL_ERROR 
                    "TINYXMLDIR path does not exist: ${TINYXML_ROOT_DIR}\n"
                    "Please verify the path and try again.")
            endif()
            
            # Check for common tinyxml2 directory structure
            set(TINYXML2_INCLUDE_DIR "${TINYXML_ROOT_DIR}/include")
            set(TINYXML2_LIB_DIR "${TINYXML_ROOT_DIR}/lib")
            
            if(NOT EXISTS "${TINYXML2_INCLUDE_DIR}")
                message(WARNING "Include directory not found: ${TINYXML2_INCLUDE_DIR}")
            endif()
            
            if(NOT EXISTS "${TINYXML2_LIB_DIR}")
                message(WARNING "Library directory not found: ${TINYXML2_LIB_DIR}")
            endif()
            
            message(STATUS "TinyXML2 include dir: ${TINYXML2_INCLUDE_DIR}")
            message(STATUS "TinyXML2 lib dir: ${TINYXML2_LIB_DIR}")
            
            # Create imported target manually
            add_library(tinyxml2::tinyxml2 INTERFACE IMPORTED)
            target_include_directories(tinyxml2::tinyxml2 INTERFACE "${TINYXML2_INCLUDE_DIR}")
            
            # Find the library file
            find_library(TINYXML2_LIBRARY 
                NAMES tinyxml2 libtinyxml2
                PATHS "${TINYXML2_LIB_DIR}"
                NO_DEFAULT_PATH
            )
            
            if(TINYXML2_LIBRARY)
                target_link_libraries(tinyxml2::tinyxml2 INTERFACE "${TINYXML2_LIBRARY}")
                message(STATUS "Found TinyXML2 library: ${TINYXML2_LIBRARY}")
            else()
                message(FATAL_ERROR "TinyXML2 library not found in: ${TINYXML2_LIB_DIR}")
            endif()
        else()
            # Windows/MSVC without TINYXMLDIR - try automatic detection
            message(STATUS "No TINYXMLDIR specified - attempting automatic TinyXML2 detection")
            message(STATUS "Note: For reliable builds on Windows, consider setting TINYXMLDIR")
            message(STATUS "Example: set TINYXMLDIR=C:/path/to/tinyxml2")
            
            # Try to find TinyXML2 without pkg-config first
            find_package(tinyxml2 QUIET)
            if(tinyxml2_FOUND)
                message(STATUS "Found TinyXML2 via find_package")
            else()
                # Fallback to pkg-config if available on Windows
                find_package(PkgConfig QUIET)
                if(PkgConfig_FOUND)
                    pkg_check_modules(TINYXML2 tinyxml2)
                    if(TINYXML2_FOUND)
                        add_library(tinyxml2::tinyxml2 INTERFACE IMPORTED)
                        target_include_directories(tinyxml2::tinyxml2 INTERFACE ${TINYXML2_INCLUDE_DIRS})
                        target_link_libraries(tinyxml2::tinyxml2 INTERFACE ${TINYXML2_LIBRARIES})
                        message(STATUS "Found TinyXML2 via pkg-config")
                    else()
                        message(WARNING "TinyXML2 not found. Consider setting TINYXMLDIR environment variable.")
                        add_library(tinyxml2::tinyxml2 INTERFACE IMPORTED)
                    endif()
                else()
                    message(WARNING "TinyXML2 not found and pkg-config not available. Consider setting TINYXMLDIR.")
                    add_library(tinyxml2::tinyxml2 INTERFACE IMPORTED)
                endif()
            endif()
        endif()
    else()
        # For non-Windows/non-MSVC builds, use standard package detection only
        # TINYXMLDIR is ignored on non-Windows platforms
        if(DEFINED TINYXMLDIR OR DEFINED ENV{TINYXMLDIR})
            message(STATUS "TINYXMLDIR specified but ignored on non-MSVC platform")
        endif()
        
        message(STATUS "Non-MSVC build - using pkg-config for TinyXML2")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(TINYXML2 REQUIRED tinyxml2)
        add_library(tinyxml2::tinyxml2 INTERFACE IMPORTED)
#        target_include_directories(tinyxml2::tinyxml2 INTERFACE ${TINYXML2_INCLUDE_DIRS})
#        target_link_libraries(tinyxml2::tinyxml2 INTERFACE ${TINYXML2_LIBRARIES})
        
        # Ensure TinyXML2 uses the same C++ standard as the project
#        target_compile_features(tinyxml2::tinyxml2 INTERFACE cxx_std_14)
        
        message(STATUS "Found TinyXML2 via pkg-config")
        message(STATUS "TinyXML2 configured to use C++14 standard")
    endif()
endif()

# libelf library package
# Platform-specific libelf detection
if(WIN32 AND MSVC)
    message(STATUS "Windows/MSVC detected - checking for LIBELFDIR")
    
    # Check for LIBELFDIR - first environment variable, then CMake variable
    # This is only available on Windows/Visual Studio
    set(LIBELF_ROOT_DIR "")
    if(DEFINED ENV{LIBELFDIR} AND NOT "$ENV{LIBELFDIR}" STREQUAL "")
        set(LIBELF_ROOT_DIR "$ENV{LIBELFDIR}")
        message(STATUS "Using LIBELFDIR from environment: ${LIBELF_ROOT_DIR}")
    elseif(DEFINED LIBELFDIR AND NOT LIBELFDIR STREQUAL "")
        set(LIBELF_ROOT_DIR "${LIBELFDIR}")
        message(STATUS "Using LIBELFDIR from CMake variable: ${LIBELF_ROOT_DIR}")
    endif()

    # If LIBELF_ROOT_DIR is set, use manual detection
    if(NOT LIBELF_ROOT_DIR STREQUAL "")
        message(STATUS "LIBELFDIR specified - using manual detection")
        
        # Validate that LIBELF_ROOT_DIR exists
        if(NOT EXISTS "${LIBELF_ROOT_DIR}")
            message(FATAL_ERROR 
                "LIBELFDIR path does not exist: ${LIBELF_ROOT_DIR}\n"
                "Please verify the path and try again.")
        endif()
        
        # Check for common libelf directory structure
        set(LIBELF_INCLUDE_DIR "${LIBELF_ROOT_DIR}/include")
        set(LIBELF_LIB_DIR "${LIBELF_ROOT_DIR}/lib")
        
        if(NOT EXISTS "${LIBELF_INCLUDE_DIR}")
            message(WARNING "Include directory not found: ${LIBELF_INCLUDE_DIR}")
        endif()
        
        if(NOT EXISTS "${LIBELF_LIB_DIR}")
            message(WARNING "Library directory not found: ${LIBELF_LIB_DIR}")
        endif()
        
        message(STATUS "libelf include dir: ${LIBELF_INCLUDE_DIR}")
        message(STATUS "libelf lib dir: ${LIBELF_LIB_DIR}")
        
        # Find the library file
        find_library(LIBELF_LIBRARY 
            NAMES elf libelf
            PATHS "${LIBELF_LIB_DIR}"
            NO_DEFAULT_PATH
        )
        
        if(LIBELF_LIBRARY)
            set(LIBELF_LINK_LIBRARIES "${LIBELF_LIBRARY}")
            set(LIBELF_INCLUDE_DIRS "${LIBELF_INCLUDE_DIR}")
            message(STATUS "Found libelf library: ${LIBELF_LIBRARY}")
        else()
            message(FATAL_ERROR "libelf library not found in: ${LIBELF_LIB_DIR}")
        endif()
    else()
        # Windows/MSVC without LIBELFDIR - provide helpful message and try pkg-config fallback
        message(WARNING "LIBELFDIR not specified on Windows. Please set LIBELFDIR environment variable or CMake variable.")
        message(STATUS "Example: set LIBELFDIR=C:/path/to/libelf")
        
        # Try pkg-config as fallback
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(LIBELF libelf)
            if(NOT LIBELF_FOUND)
                message(FATAL_ERROR "libelf not found. Please install libelf or set LIBELFDIR.")
            else()
                message(STATUS "Found libelf via pkg-config")
            endif()
        else()
            message(FATAL_ERROR "libelf not found and pkg-config not available. Please set LIBELFDIR.")
        endif()
    endif()
else()
    # For non-Windows builds, use standard package detection only
    # LIBELFDIR is ignored on non-Windows platforms
    if(DEFINED LIBELFDIR OR DEFINED ENV{LIBELFDIR})
        message(STATUS "LIBELFDIR specified but ignored on non-MSVC platform")
    endif()
    
    message(STATUS "Non-MSVC build - using pkg-config for libelf")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBELF REQUIRED libelf)
    add_library(libelf::libelf INTERFACE IMPORTED)
    target_link_libraries(libelf::libelf INTERFACE ${LIBELF_LIBRARIES})
    message(STATUS "Found libelf via pkg-config")
endif()

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
    src/crc.cpp
    src/elf.cpp
    src/format.cpp
    src/jagr2.cpp
    src/jagr3.cpp
    src/patches.cpp
    src/rom.cpp
    src/common.cpp
    src/main.cpp
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

# Ensure our executable uses C++14
target_compile_features(JiFFI2 PRIVATE cxx_std_14)

message(STATUS "Created executable target: JiFFI2")

# Include directories list
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add libelf include directories
if(LIBELF_INCLUDE_DIRS)
    list(APPEND INCLUDE_DIRS ${LIBELF_INCLUDE_DIRS})
endif()
if(NOT CLI_ONLY)
    list(APPEND INCLUDE_DIRS ${UI_OUTPUT_DIR} ${MOC_OUTPUT_DIR})
endif()
target_include_directories(JiFFI2 PRIVATE ${INCLUDE_DIRS})
message(STATUS "Include directories: ${INCLUDE_DIRS}")

# Link libraries list
#set(LINK_LIBS "")
# Add libelf libraries
if(LIBELF_LINK_LIBRARIES)
    set(LINK_LIBS ${LIBELF_LINK_LIBRARIES})
endif()
#set(LINK_LIBS libelf::libelf)
if(NOT CLI_ONLY)
    list(APPEND LINK_LIBS Qt5::Widgets tinyxml2::tinyxml2)
endif()
target_link_libraries(JiFFI2 PRIVATE ${LINK_LIBS})
message(STATUS "Link libraries: ${LINK_LIBS}")

# Visual Studio specific: Copy Qt5 DLLs to output directory
if(NOT CLI_ONLY AND WIN32 AND MSVC)
    message(STATUS "Setting up Qt5 DLL copying for Visual Studio")
    
    # Get Qt5 installation directory from the Qt5_DIR
    if(DEFINED QT5_ROOT_DIR AND NOT QT5_ROOT_DIR STREQUAL "")
        # Determine Qt5 bin directory based on Qt5_DIR format
        set(QT5_BIN_DIR "")
        if(EXISTS "${QT5_ROOT_DIR}/bin")
            # Qt5_DIR points to installation root
            set(QT5_BIN_DIR "${QT5_ROOT_DIR}/bin")
        elseif(EXISTS "${QT5_ROOT_DIR}/../../../bin")
            # Qt5_DIR points to cmake directory, go up to find bin
            get_filename_component(QT5_BIN_DIR "${QT5_ROOT_DIR}/../../../bin" ABSOLUTE)
        endif()
    else()
        # Try to get from Qt5::Core target
        get_target_property(QT5_CORE_LOCATION Qt5::Core LOCATION)
        if(QT5_CORE_LOCATION)
            get_filename_component(QT5_BIN_DIR "${QT5_CORE_LOCATION}" DIRECTORY)
        endif()
    endif()
    
    if(QT5_BIN_DIR AND EXISTS "${QT5_BIN_DIR}")
        message(STATUS "Qt5 bin directory: ${QT5_BIN_DIR}")
        
        # Define the Qt5 DLLs to copy (release versions)
        set(QT5_RELEASE_DLLS
            "Qt5Core.dll"
            "Qt5Gui.dll"
            "Qt5Widgets.dll"
        )
        
        # Define the Qt5 DLLs to copy (debug versions)
        set(QT5_DEBUG_DLLS
            "Qt5Cored.dll"
            "Qt5Guid.dll"
            "Qt5Widgetsd.dll"
        )
        
        # Copy release DLLs for Release builds
        foreach(dll ${QT5_RELEASE_DLLS})
            set(qt5_dll_path "${QT5_BIN_DIR}/${dll}")
            if(EXISTS "${qt5_dll_path}")
                add_custom_command(TARGET JiFFI2 POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,echo,copy_if_different>
                        "${qt5_dll_path}"
                        "$<TARGET_FILE_DIR:JiFFI2>/${dll}"
                    COMMENT "Copying release ${dll} to output directory (Release build only)"
                    VERBATIM
                )
                message(STATUS "Will copy release ${dll} from: ${qt5_dll_path} (Release builds)")
            else()
                message(WARNING "Release ${dll} not found at: ${qt5_dll_path}")
            endif()
        endforeach()
        
        # Copy debug DLLs for Debug builds only
        foreach(dll ${QT5_DEBUG_DLLS})
            set(qt5_dll_path "${QT5_BIN_DIR}/${dll}")
            if(EXISTS "${qt5_dll_path}")
                add_custom_command(TARGET JiFFI2 POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_if_different,echo>
                        "${qt5_dll_path}"
                        "$<TARGET_FILE_DIR:JiFFI2>/${dll}"
                    COMMENT "Copying debug ${dll} to output directory (Debug build only)"
                    VERBATIM
                )
                message(STATUS "Will copy debug ${dll} from: ${qt5_dll_path} (Debug builds only)")
            else()
                message(STATUS "Debug ${dll} not found at: ${qt5_dll_path} (optional for debug builds)")
            endif()
        endforeach()
        
    else()
        message(WARNING "Could not determine Qt5 bin directory. Qt5 DLLs will not be copied automatically.")
        message(STATUS "You may need to manually copy Qt5Core.dll, Qt5Gui.dll, and Qt5Widgets.dll to the output directory.")
    endif()
endif()

# Install the executable
install(TARGETS JiFFI2 RUNTIME DESTINATION bin)

# Clap de fin
message(STATUS "Configuration of ${PROJECT_NAME} complete")
