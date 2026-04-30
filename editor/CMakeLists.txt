set(EDITOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "${EDITOR_DIR}/*.cpp"
)

add_executable(PruneauEditor
    ${EDITOR_SOURCES}
)

target_link_libraries(PruneauEditor
    PRIVATE
        PruneauEngine
        imgui
        imguizmo
)

target_include_directories(PruneauEditor
    PRIVATE ${EDITOR_DIR}
)