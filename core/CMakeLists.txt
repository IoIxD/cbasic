project(cbcore)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB_RECURSE sources *.c)

add_library(cbcore STATIC ${sources})
