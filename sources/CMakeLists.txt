cmake_minimum_required(VERSION 3.16)
project(gradient_schemes)

set(CMAKE_CXX_STANDARD 17)
include_directories(. Mesh GradientSchemes)

# link slae library
add_library(slae STATIC IMPORTED)
set_target_properties(slae PROPERTIES
        IMPORTED_LOCATION "/home/evgen/Slaebase/libmy_project.a"
        INTERFACE_INCLUDE_DIRECTORIES "/home/evgen/Slaebase/include")
set(SLAE_LIBRARY slae)

# link vtk
find_package(VTK REQUIRED)

#link gmsh
add_library(gmsh SHARED IMPORTED)
set_target_properties(gmsh PROPERTIES
        IMPORTED_LOCATION "/home/evgen/gmshbase/lib/libgmsh.so"
        INTERFACE_INCLUDE_DIRECTORIES "/home/evgen/gmshbase/include")
set(GMSH_LIBRARY gmsh)

# creation main executable
if(${CMAKE_BUILD_TYPE} STREQUAL Release)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3 -march='native'")
endif()
file(GLOB_RECURSE MY_PROJECT_SOURCES main.cpp ../sources/Mesh/*.cpp ../sources/GradientSchemes/LeastSquareGradient.h)
add_executable(gradient_schemes ${MY_PROJECT_SOURCES})
target_link_libraries(gradient_schemes  ${SLAE_LIBRARY})
target_link_libraries(gradient_schemes  ${VTK_LIBRARIES})
target_link_libraries(gradient_schemes  ${GMSH_LIBRARY})

# creation of a special lib for gtets
file(GLOB_RECURSE MY_LIBRARY_PROJECT_SOURCES  Mesh/*.cpp Mesh/*.h )
if(${CMAKE_BUILD_TYPE} STREQUAL Release)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3 -march='native'")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
add_library(temp_dist ${MY_LIBRARY_PROJECT_SOURCES})
target_link_libraries(temp_dist PUBLIC)
target_include_directories(temp_dist PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
