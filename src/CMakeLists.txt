add_executable("Pruneau-Renderer"
    main.cpp
)

target_link_libraries("Pruneau-Renderer" PRIVATE
    glfw
    glad
    imgui
    imguizmo
    stb
    glm
)

if (UNIX)
    target_link_libraries("Pruneau-Renderer" PRIVATE GL)
endif()
