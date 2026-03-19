# Pruneau Renderer

<img width="1200" height="800" alt="pruneau2" src="https://github.com/user-attachments/assets/fa402cb6-286b-45e1-bab0-e29abebe0b37" />

Pruneau Renderer is a lightweight 3D renderer written in C++ with the goal of evolving into a full-featured game engine. For now, no stable release is there, the product is still in development.

## Librairies Used

First of all, I want to thanks all the developers of the librairies used in this project that makes it possible : 
 - [GLFW](https://www.glfw.org/)
 - [GLM](https://github.com/g-truc/glm)
 - [GLAD](https://glad.dav1d.de/)
 - [ImGui](https://github.com/ocornut/imgui)
 - [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
 - [stb](https://github.com/nothings/stb)

 Credits to [enemymouse](https://github.com/enemymouse) for the gui graphics style

## Features

- **OpenGL Rendering:** Core rendering functionality with support for framebuffers.
- **Asset Importation** : import fbx, glb, gltf, png, jpg, obj, glsl files into the engine by a simple drop !
- **GUI:** ImGui docking interface and ImGuizmo for manipulations.
- **Layer System:** Modular layer architecture for scenes, UI, and systems.
- **Event System:** Centralized event handling for window events and input.
- **Framebuffer Support:** Custom framebuffer class with color and depth attachments.

## Overview

<img width="1919" height="1001" alt="image" src="https://github.com/user-attachments/assets/79cc597b-dfb4-425c-ad62-112e91a8fa51" />


Credits to the [Blender](https://www.blender.org/) team for the suzanne model.

## Getting Started

#### For now, editor code and engine code are in the same project. As the editor is only a layer of the executable, I am planning to separate the editor and the engine layer. In the future, the engine will be a lib file and the editor an application.

#### A wiki is available 👉 [here](https://github.com/Pangauwin/Pruneau-Renderer/wiki) 👈 (still in development too)

### Prerequisites

- C++17 compatible compiler
- [CMake](https://cmake.org/) (>=3.16)
- [OpenGL](https://www.opengl.org/) (>=4.6)
- Git (in order to clone the repo and the submodules)

### Build Instructions

⚠️ This project do not build for now, as non-relative paths are used for now in this project. I consider removing them really soon.

#### WARNING #1 : This projects only builds on Linux.
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














