# Kosmic Engine [![🐧 Linux Build 🐧](https://github.com/LucasSnatiago/kosmicengine/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/LucasSnatiago/kosmicengine/actions/workflows/cmake-multi-platform.yml)

Welcome to the **Kosmic Engine** repository! A 3D game engine made with C++23 and OpenGL 3.3.

## Location

[Português - Brasil](Docs/pt_BR/README.md)

## Project Structure

- **Docs:**  
    Contains Kosmic's documentation.

- **Engine:**  
    Contains the engine's core code.

- **Sandbox:**  
    Used to test new features and identify bugs.

- **Thirdparty:**  
    Contains external libraries required for the engine to function. These dependencies can be compiled or integrated directly.

## Current Features

- **Rendering System:**  
    Supports 3D-focused rendering with shader support and basic lighting.

- **Resource Management:**  
    Asset loading and management system for 3D assets using the `assimp` library.

## How to Compile Kosmic

To compile the project, follow the steps below:

1. **Create a build directory** (inside the project directory):

     ```
     mkdir Build && cd Build
     ```

2. **Download external libraries**:

     ```
     git submodule update --init --recursive
     ```

3. **Generate the build files:**

     ```
     cmake ..
     ```

4. **Compile the project:**

     ```
     cmake --build .
     ```

     or

     ```
     make -$(nproc)
     ```

If you need to customize the build (e.g., selecting Debug or Release mode), pass the appropriate options to the `cmake` command, for example:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Contributing

Feel free to create issues or submit pull requests with improvements or fixes.  

Enjoy coding! :)
