#Project version minimum
cmake_minimum_required(VERSION 3.16.4)
#Project name
project(HelloWorldProject VERSION 1.0.0)
#source files directory
set(Source_Files
"src/main.c"
)
set(ALL_FILES ${Source_Files})
add_executable(${PROJECT_NAME} ${ALL_FILES})