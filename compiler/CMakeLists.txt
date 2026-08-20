project(cbasic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

FILE(GLOB_RECURSE sources *.cpp)

add_executable(cbasic ${sources})
