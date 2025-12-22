#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}