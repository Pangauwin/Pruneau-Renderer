set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SRC_DIR}/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "${HEADER_DIR}/*.h"
)

add_executable("Pruneau-Renderer" ${SOURCES} ${HEADERS})

target_link_libraries("Pruneau-Renderer" PRIVATE
    glfw
    glad
    imgui
    imguizmo
    stb
    glm
    assimp
)
target_include_directories("Pruneau-Renderer" PUBLIC ${HEADER_DIR})

if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)

    target_link_libraries(Pruneau-Renderer PRIVATE
        OpenGL::GL
        X11::X11
    )
endif()