cmake_minimum_required(VERSION 3.20)
project("Pruneau" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(vendor)
add_subdirectory(engine)
add_subdirectory(editor)
# add_subdirectory(sandbox) # optional

file(COPY ${CMAKE_SOURCE_DIR}/ressources DESTINATION ${CMAKE_BINARY_DIR})