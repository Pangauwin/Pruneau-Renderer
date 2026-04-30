#include "draw_test_cube.h"

#include <vector>
#include <cstdio>
#include <string>

// Simple helper: compile GL program from sources, returns 0 on error (logs to stderr)
static unsigned int CompileProgram(const char* vs_src, const char* fs_src)
{
    auto compile = [](GLenum type, const char* src) -> unsigned int {
        unsigned int id = glCreateShader(type);
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char info[1024];
            glGetShaderInfoLog(id, 1024, nullptr, info);
            std::fprintf(stderr, "Shader compile error: %s\n", info);
            glDeleteShader(id);
            return 0;
        }
        return id;
        };

    unsigned int vs = compile(GL_VERTEX_SHADER, vs_src);
    if (!vs) return 0;
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fs_src);
    if (!fs) { glDeleteShader(vs); return 0; }

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    int success = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info[1024];
        glGetProgramInfoLog(prog, 1024, nullptr, info);
        std::fprintf(stderr, "Program link error: %s\n", info);
        glDeleteProgram(prog);
        prog = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

// Draw a unit cube (-0.5..+0.5) into the provided framebuffer.
// Assumes an active OpenGL context and glad already initialized.
void Debug_DrawCubeToFramebuffer( const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& perspective)
{
    // Simple shader matching your project's attribute locations:
    // location 0 = position, 1 = normal, 2 = uv_coord
    const char* vertex_shader = R"(
        #version 460 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv_coord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 perspective;

        out vec3 vNormal;
        out vec2 vUV;

        void main()
        {
            vNormal = mat3(transpose(inverse(model))) * normal;
            vUV = uv_coord;
            gl_Position = perspective * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragment_shader = R"(
        #version 460 core
        in vec3 vNormal;
        in vec2 vUV;
        out vec4 FragColor;
        void main()
        {
            vec3 n = normalize(vNormal);
            float lighting = max(dot(n, normalize(vec3(0.5, 1.0, 0.3))), 0.2);
            FragColor = vec4(vec3(0.6, 0.8, 1.0) * lighting, 1.0);
        }
    )";

    unsigned int prog = CompileProgram(vertex_shader, fragment_shader);
    if (!prog)
    {
        std::fprintf(stderr, "Failed to create debug shader program\n");
        return;
    }

    // Cube: position(3), normal(3), uv(2) -> 8 floats per vertex
    float vertices[] = {
        // positions         // normals          // uvs
        -0.5f, -0.5f, -0.5f,  0,  0, -1,  0.0f, 0.0f, // 0
         0.5f, -0.5f, -0.5f,  0,  0, -1,  1.0f, 0.0f, // 1
         0.5f,  0.5f, -0.5f,  0,  0, -1,  1.0f, 1.0f, // 2
        -0.5f,  0.5f, -0.5f,  0,  0, -1,  0.0f, 1.0f, // 3

        -0.5f, -0.5f,  0.5f,  0,  0, 1,  0.0f, 0.0f, // 4
         0.5f, -0.5f,  0.5f,  0,  0, 1,  1.0f, 0.0f, // 5
         0.5f,  0.5f,  0.5f,  0,  0, 1,  1.0f, 1.0f, // 6
        -0.5f,  0.5f,  0.5f,  0,  0, 1,  0.0f, 1.0f, // 7

        -0.5f,  0.5f,  0.5f, -1,  0,  0,  1.0f, 0.0f, // 8  (left)
        -0.5f,  0.5f, -0.5f, -1,  0,  0,  1.0f, 1.0f, // 9
        -0.5f, -0.5f, -0.5f, -1,  0,  0,  0.0f, 1.0f, //10
        -0.5f, -0.5f,  0.5f, -1,  0,  0,  0.0f, 0.0f, //11

         0.5f,  0.5f,  0.5f,  1,  0,  0,  1.0f, 0.0f, //12 (right)
         0.5f,  0.5f, -0.5f,  1,  0,  0,  1.0f, 1.0f, //13
         0.5f, -0.5f, -0.5f,  1,  0,  0,  0.0f, 1.0f, //14
         0.5f, -0.5f,  0.5f,  1,  0,  0,  0.0f, 0.0f, //15

        -0.5f, -0.5f, -0.5f,  0, -1,  0,  0.0f, 1.0f, //16 (bottom)
         0.5f, -0.5f, -0.5f,  0, -1,  0,  1.0f, 1.0f, //17
         0.5f, -0.5f,  0.5f,  0, -1,  0,  1.0f, 0.0f, //18
        -0.5f, -0.5f,  0.5f,  0, -1,  0,  0.0f, 0.0f, //19

        -0.5f,  0.5f, -0.5f,  0,  1,  0,  0.0f, 1.0f, //20 (top)
         0.5f,  0.5f, -0.5f,  0,  1,  0,  1.0f, 1.0f, //21
         0.5f,  0.5f,  0.5f,  0,  1,  0,  1.0f, 0.0f, //22
        -0.5f,  0.5f,  0.5f,  0,  1,  0,  0.0f, 0.0f  //23
    };

    unsigned int indices[] = {
        // back
        0, 1, 2, 2, 3, 0,
        // front
        4, 5, 6, 6, 7, 4,
        // left
        8, 9,10,10,11, 8,
        // right
       12,13,14,14,15,12,
       // bottom
      16,17,18,18,19,16,
      // top
     20,21,22,22,23,20
    };

    unsigned int VAO = 0, VBO = 0, EBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // uv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    glUseProgram(prog);
    GLint loc = glGetUniformLocation(prog, "model");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
    loc = glGetUniformLocation(prog, "view");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(prog, "perspective");
    if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(perspective));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // cleanup
    glDeleteProgram(prog);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

// Example usage (call from Renderer::PostRender or a one-off debug place):
//   glm::mat4 model = glm::mat4(1.0f);
//   glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f));
//   glm::mat4 proj = glm::perspective(glm::radians(70.0f), float(fb.width) / float(fb.height), 0.1f, 100.0f);
//   Debug_DrawCubeToFramebuffer(m_frame_buffer, model, view, proj);