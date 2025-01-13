cmake_minimum_required(VERSION 3.15)
project(HelloWorldProject VERSION 1.0.0)

# Set the cross-compiler
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# Set the sysroot
set(CMAKE_SYSROOT ~/bbb-sysroot/)

#source files directory
set(Source_Files
    "src/main.c"
)
set(SRC_FILES ${Source_Files})

# Specify target include directories and libraries
include_directories(${CMAKE_SYSROOT}/usr/include)
link_directories(${CMAKE_SYSROOT}/usr/lib)

# Add an executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(main m)
