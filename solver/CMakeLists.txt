cmake_minimum_required(VERSION 3.20 FATAL_ERROR)


if(DEFINED ENV{WM_PROJECT})
    MESSAGE("Using $ENV{WM_PROJECT}-$ENV{WM_PROJECT_VERSION}")
    SET(WM_PATH ${CMAKE_SOURCE_DIR})
    MESSAGE("The project is located at ${CMAKE_SOURCE_DIR}")
else()
    message(FATAL_ERROR "The OpenFOAM bashrc is not sourced")
endif()

project(
        IBpimpleFoam.C
        VERSION
        0.1
        DESCRIPTION
        "Cmake in Clion"
        LANGUAGES
        CXX
)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(OpenFOAM_Version $ENV{WM_PROJECT_VERSION})
SET(OpenFOAM_DIR $ENV{WM_PROJECT_DIR})
SET(OpenFOAM_LIB_DIR $ENV{FOAM_LIBBIN})
SET(OpenFOAM_SRC $ENV{FOAM_SRC})

#MESSAGE(${OpenFoam_Version}\n
#        ${OpenFoam_DIR}\n
#        ${OpenFoam_LIB_DIR}\n
#        ${OpenFoam_SRC}\n
#)

SET(DEFINITIONS_COMPILE "-std=c++14 -m64 -pthread -DOPENFOAM=2312 -DWM_DP -DWM_LABEL_SIZE=32
-Wall -Wextra -Wold-style-cast -Wnon-virtual-dtor -Wno-unused-parameter
-Wno-invalid-offsetof -Wno-attributes -Wno-unknown-pragmas -O3
-DNoRepository -ftemplate-depth-100
-fPIC -DIMPLEMENT_ACTIVATION -execute,-undefined,dynamic_lookup")

add_definitions("${DEFINITIONS_COMPILE}")


include_directories(.
        /usr/lib/x86_64-linux-gnu/openmpi/include
        ${OpenFOAM_SRC}/OpenFOAM/lnInclude
        ${OpenFOAM_SRC}/TurbulenceModels/turbulenceModels/lnInclude
        ${OpenFOAM_SRC}/OSspecific/POSIX/lnInclude
        ${OpenFOAM_SRC}/finiteVolume/lnInclude
        ${OpenFOAM_SRC}/meshTools/lnInclude
        ${OpenFOAM_SRC}/surfMesh/lnInclude
        ${OpenFOAM_SRC}/dynamicMesh/lnInclude
        ${OpenFOAM_SRC}/dynamicFvMesh/lnInclude

)

link_directories(${OpenFOAM_LIB_DIR} ${OpenFOAM_LIB_DIR}/dummy ${OpenFOAM_LIB_DIR}/${PATH_LIB_OPENMPI})

ADD_EXECUTABLE(${PROJECT_NAME}
        IBpimpleFoam.C)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} OpenFOAM dl m Pstream immersedBoundary/finiteVolume fvOptions meshTools dynamicMesh)
