cmake_minimum_required(VERSION 3.21)


# setting up the vcpkg toolpath. 
execute_process(COMMAND which vcpkg OUTPUT_VARIABLE VCPKG_COMMAND_PATH)
string(REGEX REPLACE "vcpkg\n" "" VCPKG_PATH "${VCPKG_COMMAND_PATH}")
set(VCPKG_TOOLCHAIN_PATH ${VCPKG_PATH}scripts/buildsystems/vcpkg.cmake)
message("vcpkg path: "${VCPKG_TOOLCHAIN_PATH})
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_PATH} CACHE STRING "TEST") 

# set({Project_NAME} "Benchmark")
# setting project and project library variable names.
set(Project "Benchmark")

# settting the project name.
project(${Project} C CXX)


# setting variables for PETSC

set(PETSC_SOURCE_DIR "${CMAKE_SOURCE_DIR}/extLibs/petsc/src")
set(PETSC_INSTALL_DIR "${CMAKE_SOURCE_DIR}/extLibs/petsc/install")
set(PETSC_REPO "https://gitlab.com/petsc/petsc.git")
set(PETSC_BUILD_TYPE "release")
set(PETSC_GIT_TAG "release")
set(ENV{PKG_CONFIG_PATH} ${PETSC_INSTALL_DIR}/lib/pkgconfig)


############ Find Package Stuffs########################################################
########################################################################################
# check if openMP compilers are available and link them
find_package(OpenMP 4.0)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(Armadillo CONFIG REQUIRED)

include(ExternalProject)
find_package(PkgConfig REQUIRED)
pkg_search_module(PETSC IMPORTED_TARGET PETSc)

if(NOT PETSC_FOUND)
    file(MAKE_DIRECTORY ${PETSC_SOURCE_DIR})
    execute_process(
        COMMAND echo "Starting the PETSc Cloning from gitlab"
        COMMAND git clone -b ${PETSC_GIT_TAG} ${PETSC_REPO} ./src
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/extLibs/petsc 
        )
    execute_process(
        COMMAND ./configure --prefix=${PETSC_INSTALL_DIR} --with-cc=gcc --with-cxx=g++ --with-fc=0 --download-f2cblaslapack --download-mpich
        WORKING_DIRECTORY ${PETSC_SOURCE_DIR}
        )    
    execute_process(
        COMMAND make PETSC_DIR=${PETSC_SOURCE_DIR} PETSC_ARCH=arch-linux-c-debug all
        WORKING_DIRECTORY ${PETSC_SOURCE_DIR}
        )    
    execute_process(
        COMMAND make PETSC_DIR=${PETSC_SOURCE_DIR} PETSC_ARCH=arch-linux-c-debug all install
        WORKING_DIRECTORY ${PETSC_SOURCE_DIR}
        )
    file(REMOVE_RECURSE ${PETSC_SOURCE_DIR})
    pkg_search_module(PETSC IMPORTED_TARGET PETSc)
endif()

# specifying the header directory
include_directories(
    ${ProjectLib} "${CMAKE_SOURCE_DIR}/src/headers"
    )
include_directories(${ARMADILLO_INCLUDE_DIRS})

# generating executable program by specifying the location of the file

######### Generating Executables #######################################################
########################################################################################
add_executable(
    ${Project} src/main.cpp
    )

add_subdirectory(src)

target_link_libraries(
        ${Project} 
        PUBLIC ${ARMADILLO_LIBRARIES}
        PkgConfig::PETSC
        )

