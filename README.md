  ##  Julia Set Renderer
  Renders Julia sets using OpenGL 4.6 fragment shaders in VSCode.
##  Math Overview
For complex number $z$ and constant $c$, iterate:
$z_{n+1} = z_n^2 + c$

where ( z ) is a complex number in the plane. A point ( z_0 ) belongs to the set if the sequence remains bounded (i.e., $(|z_n| \leq 2 $)) after many iterations. The fragment shader evaluates each pixel’s coordinates as ( z_0 ), iterates the function, and colors based on escape time or boundedness.

Iteration: ( $z_{n+1} = z_n^2 + c $), typically 100–1000 iterations.(here, 256 )
Escape Condition: If ( |z_n| > 2 ), the point escapes; color by iteration count.
Coloring: Map iteration count to RGB for visualization.

##  Setup

Install VSCode with OpenGL 4.6 support (e.g., via GLSL extension).

Clone repo: git clone <repo-url>.

Run with an OpenGL-compatible environment (e.g., GLFW, GLAD).
#### 1. Install vcpkg

First, you need to install vcpkg itself. This involves cloning the repository and running the bootstrap script.

*   **Clone the repository:**
    ```bash
    git clone https://github.com/microsoft/vcpkg
    ```

*   **Run the bootstrap script:**
    *   On **Windows**:
        ```bash
        .\vcpkg\bootstrap-vcpkg.bat
        ```
    *   On **Linux or macOS**:
        ```bash
        ./vcpkg/bootstrap-vcpkg.sh
        ```

#### 2. Install Dependencies

Once vcpkg is set up, you can install the required libraries. For 64-bit systems, it is recommended to specify the `x64-windows` triplet.

*   **Install glfw, glad, and glm:**
    ```bash
    vcpkg install glfw3 glad glm
    ```
    *For 64-bit Windows, use:*
    ```bash
    vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows
    ```

#### 3. Execute the CMake 
    cmake ..
    cd build && cmake --build



##  Demo
![2025-07-03 22-43-33](https://github.com/user-attachments/assets/d8b2347d-dbef-4c4e-ac03-c5f9fc2d3113)

with only fragment shaders


##  Benchmarks 

FPS:  150-300 (only using fragment shader)

  330-400 (newer version with compute shader)

GPU : Nvidia GeForce MX330 (2GB VRAM)

note: it can be further optimized using compute shaders but..

for now i am just too lazy for it...


edit: well.. i broke the laziness the next day...

