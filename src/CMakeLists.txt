add_executable("Pruneau-Renderer"
    main.cpp
 "platform/window.h" "platform/window.cpp" "core/application.h" "core/application.cpp" "core/layer.h" "core/layer_stack.h" "core/layer_stack.cpp" "layers/engine_layer.h" "layers/engine_layer.cpp" "renderer/renderer.h" "renderer/renderer.cpp" "core/event/event.h" "core/event/event_dispatcher.h" "renderer/frame_buffer.h" "renderer/frame_buffer.cpp")

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
