# Pruneau Renderer

A lightweight 3D renderer written in C++ with the goal of evolving into a full-featured game engine. For now, no stable release is there, the product is still in development.

## Librairies Used

A huge thanks to all the developers of the librairies used in this project : 
 - [GLFW](https://www.glfw.org/)
 - [GLM](https://github.com/g-truc/glm)
 - [GLAD](https://glad.dav1d.de/)
 - [ImGui](https://github.com/ocornut/imgui)
 - [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
 - [stb](https://github.com/nothings/stb)

## Features

- **OpenGL Rendering:** Core rendering functionality with support for framebuffers.
- **Windowing:** Managed using GLFW.
- **GUI:** ImGui docking interface and ImGuizmo for manipulations.
- **Layer System:** Modular layer architecture for scenes, UI, and systems.
- **Event System:** Centralized event handling for window events and input.
- **Framebuffer Support:** Custom framebuffer class with color and depth attachments.
- **Scalable Design:** Initial support for 2D and 3D mesh rendering.

## Getting Started

#### For now, editor code and engine code are in the same project. As the editor is only a layer of the executable, I am planning to separate the editor and the engine layer. In the future, the engine will be a lib file and the editor an application.

### Prerequisites

- C++17 compatible compiler
- [CMake](https://cmake.org/) (>=3.16)
- [OpenGL](https://www.opengl.org/) (>=4.6)
- Git (in order to clone the repo and the submodules)

### Build Instructions

#### WARNING #1 : This project only builds on Windows for now (Linux support is coming, but not for now)
#### WARNING #2 : This is NOT a stable version, the project is still in development, expect crashes/non working features
#### WARNING #3 : This project only works with Visual Studio 2026 and MSVC, don't expect it to work on other compilers

1. Clone the repository:
    ```bash
    git clone https://github.com/Pangauwin/Pruneau-Renderer.git --recurse-submodules
    cd Pruneau-Renderer
    ```

2. Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

3. Generate the project using CMake:
    ```bash
    cmake ..
    ```

4. Build the project:
    ```bash
    cmake --build .
    ```

## Prebuilt binairies
Prebuilt binairies will arrive as soon as the first version (even if unstable) is released. As explained, it will first come on Windows and then on Linux.
There will be one binary for each major version release.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.







