# PackageBuilder.cmake
# Author: Noah Oblath
# Parts of this script are based on work done by Sebastian Voecking and Marco Haag in the Kasper package
# Convenient macros and default variable settings for the Nymph-based build.
#
# Requires: CMake v3.1 or better (CMAKE_CXX_STANDARD)

# CMake policies
cmake_policy( SET CMP0011 NEW )
cmake_policy( SET CMP0012 NEW ) # how if-statements work
cmake_policy( SET CMP0042 NEW ) # rpath on mac os x
cmake_policy( SET CMP0048 NEW ) # version in project()

# check if this is a stand-alone build
set( PBUILDER_STANDALONE FALSE CACHE INTERNAL "Flag for whether or not this is a stand-alone build" )
set( PBUILDER_CHILD_NAME_EXTENSION "${PROJECT_NAME}" CACHE INTERNAL "Submodule library name modifier" )
if( ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
    set( PBUILDER_STANDALONE TRUE )
    
    if( CMAKE_GENERATOR MATCHES ".*(Make|Ninja).*" AND NOT CMAKE_BUILD_TYPE )
  		set( CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel" FORCE )
  		message( STATUS "CMAKE_BUILD_TYPE not specified. Using ${CMAKE_BUILD_TYPE} build" )
    endif()

    # option to force linking when using g++
    if( CMAKE_COMPILER_IS_GNUCXX )
        option( GCC_FORCE_LINKING "Fix linker errors with some GCC versions by adding the --no-as-needed flag" ON )
        if( GCC_FORCE_LINKING )
            set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed" )
        endif( GCC_FORCE_LINKING )
    endif( CMAKE_COMPILER_IS_GNUCXX )
endif( ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )

# define a variable pointing to the directory containing this file
set( PBUILDER_DIR ${CMAKE_CURRENT_LIST_DIR} )

# preprocessor defintion for debug build
if( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )
    add_definitions(-D${PROJECT_NAME}_DEBUG )
else( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )
    remove_definitions(-D${PROJECT_NAME}_DEBUG )    
endif( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )

# Setup the default install prefix
# This gets set to the binary directory upon first configuring.
# If the user changes the prefix, but leaves the flag OFF, then it will remain as the user specified.
# If the user wants to reset the prefix to the default (i.e. the binary directory), then the flag should be set ON.
if( NOT DEFINED SET_INSTALL_PREFIX_TO_DEFAULT )
    set( SET_INSTALL_PREFIX_TO_DEFAULT ON )
endif( NOT DEFINED SET_INSTALL_PREFIX_TO_DEFAULT )
if( SET_INSTALL_PREFIX_TO_DEFAULT )
    set( CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR} CACHE PATH "Install prefix" FORCE )
    set( SET_INSTALL_PREFIX_TO_DEFAULT OFF CACHE BOOL "Reset default install path when when configuring" FORCE )
endif( SET_INSTALL_PREFIX_TO_DEFAULT )

# install subdirectories
set( INCLUDE_INSTALL_SUBDIR "include/${PROJECT_NAME}" CACHE PATH "Install subdirectory for headers" )
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( LIB_INSTALL_SUBDIR "bin" CACHE PATH "Install subdirectory for libraries" )
else( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( LIB_INSTALL_SUBDIR "lib" CACHE PATH "Install subdirectory for libraries" )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
set( BIN_INSTALL_SUBDIR "bin" CACHE PATH "Install subdirectory for binaries" )
set( CONFIG_INSTALL_SUBDIR "config" CACHE PATH "Install subdirectory for config files" )

set( INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_SUBDIR}" )
set( LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_SUBDIR}" )
set( BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${BIN_INSTALL_SUBDIR}" )
set( CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CONFIG_INSTALL_SUBDIR}" )

# flag for building test programs
option( ${PROJECT_NAME}_ENABLE_TESTING "Turn on or off the building of test programs" OFF )

# flag for building executables (other than test programs)
# this is particularly useful if a project is used multiple times and installed in a general location, where executables would overwrite each other.
option( ${PROJECT_NAME}_ENABLE_EXECUTABLES "Turn on or off the building of executables (other than test programs)" ON )

# flag for using C++11
option( USE_CPP11 "Flag for building with C++11" ON )
option( USE_CPP14 "Flag for building with C++14" OFF )
if( USE_CPP14 )
    add_definitions( -DUSE_CPP14 )
    add_definitions( -DUSE_CPP11 )
    set( CMAKE_CXX_STANDARD 14 )
elseif( USE_CPP11 )
    add_definitions( -DUSE_CPP11 )
    set( CMAKE_CXX_STANDARD 11 )
else()
    remove_definitions( -DUSE_CPP11 )
    remove_definitions( -DUSE_CPP14 )
    set( CMAKE_CXX_STANDARD 98 )
endif()

# build shared libraries
set( BUILD_SHARED_LIBS ON )

# global property to hold the names of nymph library targets
set_property( GLOBAL PROPERTY ${PROJECT_NAME}_LIBRARIES )

# turn on RPATH for Mac OSX
set( CMAKE_MACOSX_RPATH ON )

# add the library install directory to the rpath
SET(CMAKE_INSTALL_RPATH "${LIB_INSTALL_DIR}" )

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )

set( LIB_POSTFIX )
set( INC_PREFIX )

# in windows, disable the min and max macros
if( WIN32 )
    add_definitions( -DNOMINMAX )
endif( WIN32 )

##########
# MACROS #
##########

# Conveniece function for overriding the value of an option (aka a cached bool variable)
macro( set_option VARIABLE VALUE )
    set( ${VARIABLE} ${VALUE} CACHE BOOL "" FORCE )
endmacro()

# This should be called immediately after setting the project name
macro( pbuilder_prepare_project )
    # define the variables to describe the package (will go in the [ProjectName]Config.hh file)
    set( ${PROJECT_NAME}_PACKAGE_NAME "${PROJECT_NAME}" )
    set( ${PROJECT_NAME}_PACKAGE_STRING "${PROJECT_NAME} ${${PROJECT_NAME}_VERSION}" )
    
    # Configuration header file
    if( EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.hh.in )
        configure_file( 
            ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.hh.in
            ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hh
        )
        # Add the binary tree to the search path for include files so that the config file is found during the build
        include_directories( ${PROJECT_BINARY_DIR} )
    endif( EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.hh.in )
    
    # if git is used, get the commit SHA1
    find_package( Git )
    if( GIT_FOUND )
        execute_process( COMMAND ${GIT_EXECUTABLE} rev-parse -q HEAD  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE ${PROJECT_NAME}_GIT_COMMIT  OUTPUT_STRIP_TRAILING_WHITESPACE )
        execute_process( COMMAND ${GIT_EXECUTABLE} describe --tags --long  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE ${PROJECT_NAME}_GIT_DESCRIBE  OUTPUT_STRIP_TRAILING_WHITESPACE )
    endif( GIT_FOUND )
    
endmacro()

macro( pbuilder_add_submodule SM_NAME SM_LOCATION )
    #message( STATUS "Checking submodule ${SM_NAME}_FOUND and ${CMAKE_CURRENT_LIST_DIR}/${SM_LOCATION}" )
    if( NOT ${SM_NAME}_FOUND OR "${${SM_NAME}_LOCATION}" STREQUAL "${CMAKE_CURRENT_LIST_DIR}/${SM_LOCATION}" )
        message( STATUS "Building submodule ${SM_NAME} at ${CMAKE_CURRENT_LIST_DIR}/${SM_LOCATION}" )

        if( NOT DEFINED PARENT_LIB_NAME_SUFFIX )
            set( PARENT_LIB_NAME_SUFFIX "_${PROJECT_NAME}" CACHE INTERNAL "Library name suffix for submodules" )
        else( NOT DEFINED PARENT_LIB_NAME_SUFFIX )
            set( PARENT_LIB_NAME_SUFFIX "_${PROJECT_NAME}${PARENT_LIB_NAME_SUFFIX}" CACHE INTERNAL "" )
        endif( NOT DEFINED PARENT_LIB_NAME_SUFFIX )
        #message( STATUS "PARENT_LIB_NAME_SUFFIX being set for SM ${SM_NAME}: ${PARENT_LIB_NAME_SUFFIX}" )
        
        if( NOT DEFINED PARENT_INC_DIR_PATH )
            set( PARENT_INC_DIR_PATH "/${SM_NAME}" CACHE INTERNAL "Include directory path for submodules" )
        else( NOT DEFINED PARENT_INC_DIR_PATH )
            set( PARENT_INC_DIR_PATH "${PARENT_INC_DIR_PATH}/${SM_NAME}" CACHE INTERNAL "" )
        endif( NOT DEFINED PARENT_INC_DIR_PATH )
        #message( STATUS "PARENT_INC_DIR_PATH being set for SM ${SM_NAME}: ${PARENT_INC_DIR_PATH}" )
        
        set( ${SM_NAME}_PARENT_LIB_NAME_SUFFIX ${PARENT_LIB_NAME_SUFFIX} CACHE INTERNAL "Scoped library name suffix for submodules" )
        set( ${SM_NAME}_PARENT_INC_DIR_PATH ${PARENT_INC_DIR_PATH} CACHE INTERNAL "Scoped include directory path for submodules" )
        #message( STATUS "Just created ${SM_NAME}_PARENT_LIB_NAME_SUFFIX as ${${SM_NAME}_PARENT_LIB_NAME_SUFFIX}" )
        
        add_subdirectory( ${SM_LOCATION} )
        message( STATUS "SM ${SM_NAME} cached variables:" )
        message( STATUS "${SM_NAME}_FOUND: ${${SM_NAME}_FOUND}" )
        message( STATUS "${SM_NAME}_LOCATION: ${${SM_NAME}_LOCATION}" )
        message( STATUS "${SM_NAME}_LIBRARIES: ${${SM_NAME}_LIBRARIES}" )
        message( STATUS "${SM_NAME}_LIBRARY_DIR: ${${SM_NAME}_LIBRARY_DIR}" )
        message( STATUS "${SM_NAME}_INCLUDE_DIR: ${${SM_NAME}_INCLUDE_DIR}" )
        message( STATUS "${SM_NAME}_DEP_INCLUDE_DIRS: ${${SM_NAME}_DEP_INCLUDE_DIRS}" )
        
        pbuilder_add_submodule_libraries( ${${SM_NAME}_LIBRARIES} )
        
        include_directories( BEFORE ${${SM_NAME}_DEP_INCLUDE_DIRS} )
        
        unset( PARENT_LIB_NAME_SUFFIX CACHE )
        unset( ${SM_NAME}_PARENT_LIB_NAME_SUFFIX CACHE )
    
        unset( PARENT_INC_DIR_PATH CACHE )
        unset( ${SM_NAME}_PARENT_INC_DIR_PATH )
        
    endif( NOT ${SM_NAME}_FOUND OR "${${SM_NAME}_LOCATION}" STREQUAL "${CMAKE_CURRENT_LIST_DIR}/${SM_LOCATION}" )
endmacro()

macro( pbuilder_add_ext_libraries )
    list( APPEND EXTERNAL_LIBRARIES ${ARGN} )
endmacro()

macro( pbuilder_add_submodule_libraries )
    list( APPEND EXTERNAL_LIBRARIES ${ARGN} )
    list( APPEND SUBMODULE_LIBRARIES ${ARGN} )
endmacro()

macro( pbuilder_library LIB_BASENAME SOURCES PROJECT_LIBRARIES )
    #message( STATUS "Building library ${LIB_BASENAME}; PARENT_LIB_NAME_SUFFIX is ${PARENT_LIB_NAME_SUFFIX}; ${PROJECT_NAME}_PARENT_LIB_NAME_SUFFIX is ${${PROJECT_NAME}_PARENT_LIB_NAME_SUFFIX}" )
    set( FULL_LIB_NAME "${LIB_BASENAME}${${PROJECT_NAME}_PARENT_LIB_NAME_SUFFIX}" )
    #message( STATUS "lib basename: ${LIB_BASENAME}" )
    #message( STATUS "full lib name: ${FULL_LIB_NAME}" )

    set( FULL_PROJECT_LIBRARIES )
    foreach( project_lib ${${PROJECT_LIBRARIES}} )
        list( APPEND FULL_PROJECT_LIBRARIES "${project_lib}${${PROJECT_NAME}_PARENT_LIB_NAME_SUFFIX}" )
    endforeach( project_lib )
    #message( STATUS "project libraries( lib ): ${FULL_PROJECT_LIBRARIES}" )

    message( STATUS "pbuilder: will build library <${FULL_LIB_NAME}>" )
    add_library( ${FULL_LIB_NAME} ${${SOURCES}} )
    target_compile_options( ${FULL_LIB_NAME} INTERFACE ${GLOBAL_COMPILE_OPTIONS} )
    target_link_libraries( ${FULL_LIB_NAME} ${FULL_PROJECT_LIBRARIES} ${EXTERNAL_LIBRARIES} )
    pbuilder_install_libraries( ${FULL_LIB_NAME})
endmacro()

macro( pbuilder_install_libraries )
    #message( STATUS "installing libs: ${ARGN}" )
    install( TARGETS ${ARGN} EXPORT ${PROJECT_NAME}Targets DESTINATION ${LIB_INSTALL_DIR} )
    #list( APPEND ${PROJECT_NAME}_LIBRARIES ${ARGN} )
    set_property( GLOBAL APPEND PROPERTY ${PROJECT_NAME}_LIBRARIES ${ARGN} )
    set_target_properties( ${ARGN} PROPERTIES INSTALL_NAME_DIR ${LIB_INSTALL_DIR} )
endmacro()

macro( pbuilder_executables PROGRAMS PROJECT_LIBRARIES )
    set( FULL_PROJECT_LIBRARIES )
    foreach( project_lib ${${PROJECT_LIBRARIES}} )
        list( APPEND FULL_PROJECT_LIBRARIES "${project_lib}${${PROJECT_NAME}_PARENT_LIB_NAME_SUFFIX}" )
    endforeach( project_lib )
    message( STATUS "programs: ${${PROGRAMS}}" )
    message( STATUS "project libraries( exe): ${FULL_PROJECT_LIBRARIES}" )
    message( STATUS "external libraries: ${EXTERNAL_LIBRARIES}" )

    message( STATUS "pbuilder: will build the following executables: ${${PROGRAMS}}" )
    foreach( program ${${PROGRAMS}} )
        if( NOT TARGET ${program} )
            if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cc )
                add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cc )
            elseif( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cxx )
                add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cxx )
            elseif( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cpp )
                add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cpp )
            elseif( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${program}.c )
                add_executable( ${program} ${CMAKE_CURRENT_SOURCE_DIR}/${program}.c )
            else ()
                message( FATAL_ERROR "No ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cc, .cxx, .cpp, or .c to be built")
            endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${program}.cc)
            target_compile_options( ${program} INTERFACE ${GLOBAL_COMPILE_OPTIONS} )
            target_link_libraries( ${program} ${FULL_PROJECT_LIBRARIES} ${EXTERNAL_LIBRARIES} )
            pbuilder_install_executables( ${program} )
        endif( NOT TARGET ${program} )
    endforeach( program )
endmacro()

macro( pbuilder_install_executables )
    install( TARGETS ${ARGN} EXPORT ${PROJECT_NAME}Targets DESTINATION ${BIN_INSTALL_DIR} )
endmacro()

macro( pbuilder_install_headers )
    #message( STATUS "installing headers in ${INCLUDE_INSTALL_DIR}${${PROJECT_NAME}_PARENT_INC_DIR_PATH}" )
    install( FILES ${ARGN} DESTINATION ${INCLUDE_INSTALL_DIR}${${PROJECT_NAME}_PARENT_INC_DIR_PATH} )
endmacro()

macro( pbuilder_install_header_dirs FILE_PATTERN )
    install( DIRECTORY ${ARGN} DESTINATION ${INCLUDE_INSTALL_DIR}${${PROJECT_NAME}_PARENT_INC_DIR_PATH} FILES_MATCHING PATTERN "${FILE_PATTERN}" )
endmacro()

macro( pbuilder_install_config )
    install( FILES ${ARGN} DESTINATION ${CONFIG_INSTALL_DIR} )
endmacro()

macro( pbuilder_install_files DEST_DIR )
    install( FILES ${ARGN} DESTINATION ${DEST_DIR} )
endmacro()

# This should be called AFTER all subdirectories with libraries have been called, and all include directories added.
macro( pbuilder_install_config_files )
    # Configuration header file
    if( EXISTS ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hh )
        # Install location for the configuration header
        pbuilder_install_headers( ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hh )
    endif( EXISTS ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hh )

    # CMake configuration file
    get_property(${PROJECT_NAME}_LIBRARIES GLOBAL PROPERTY ${PROJECT_NAME}_LIBRARIES )
    if( EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in )
        # the awkwardness of the following four lines is because for some reason cmake wouldn't just go from .cmake.tmp to .cmake when I tested it.
        configure_file(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}Config.cmake.tmp @ONLY )
        configure_file(${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}Config.cmake.tmp ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}Config.cmake.ppp @ONLY )
        file( REMOVE ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}Config.cmake.tmp )
        file( RENAME ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}Config.cmake.ppp ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}Config.cmake )
    endif( EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in )
endmacro()

macro( pbuilder_variables_for_parent )
    if( NOT ${PBUILDER_STANDALONE} )
        set( ${PROJECT_NAME}_FOUND TRUE CACHE INTERNAL "" )
        set( ${PROJECT_NAME}_LOCATION ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "" )
        get_property( LIBRARIES GLOBAL PROPERTY ${PROJECT_NAME}_LIBRARIES )
        set( ${PROJECT_NAME}_LIBRARIES ${LIBRARIES} ${SUBMODULE_LIBRARIES} CACHE INTERNAL "" )
        set( ${PROJECT_NAME}_LIBRARY_DIR ${LIB_INSTALL_DIR} CACHE INTERNAL "" )
        set( ${PROJECT_NAME}_INCLUDE_DIR ${INCLUDE_INSTALL_DIR} CACHE INTERNAL "" )
        get_property( DEP_INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES )
        list( REMOVE_DUPLICATES DEP_INCLUDE_DIRS )
        set( ${PROJECT_NAME}_DEP_INCLUDE_DIRS ${DEP_INCLUDE_DIRS} CACHE INTERNAL "" )
        get_directory_property( COMPILE_DEFINITIONS COMPILE_DEFINITIONS )
        set( ${PROJECT_NAME}_COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} CACHE INTERNAL "" )
    endif( NOT ${PBUILDER_STANDALONE} )
endmacro()
