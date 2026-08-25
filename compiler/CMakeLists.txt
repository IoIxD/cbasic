project(cbasic)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

FILE(GLOB_RECURSE sources *.cpp)

add_executable(cbasic ${sources})

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader
    AllTargetsInfos
    AllTargetsDescs
    AllTargetsCodeGens
    AllTargetsAsmParsers
    AsmPrinter
    MC
)
message(STATUS "llvm libs ${llvm_libs}")
target_link_libraries(${PROJECT_NAME} ${llvm_libs})
