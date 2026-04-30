set(ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${ENGINE_DIR}/*.cpp"
)

file(GLOB_RECURSE ENGINE_HEADERS
    "${ENGINE_DIR}/*.h"
)

add_library(PruneauEngine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(PruneauEngine
    PUBLIC ${ENGINE_DIR}
)

target_link_libraries(PruneauEngine
    PUBLIC
        glfw
        glad
        glm
        stb
        assimp
        imgui
        imguizmo
)

if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)

    target_link_libraries(PruneauEngine PUBLIC
        OpenGL::GL
        X11::X11
    )
endif()