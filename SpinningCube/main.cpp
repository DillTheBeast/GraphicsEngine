//g++ -std=c++17 \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/Main.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/src/glad.c \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/EBO.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/VAO.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/VBO.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/shaderClass.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/imgui.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/imgui_demo.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/imgui_draw.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/imgui_widgets.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/imgui_tables.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/backends/imgui_impl_opengl3.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/backends/imgui_impl_glfw.cpp \
    -o main \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/glm \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/SpinningCube/include/imgui/backends \
    -L/Users/dillonmaltese/Documents/GitHub/OpenGL/lib \
    -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

#include <iostream>
#include <limits>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

bool cubeMove = true;
bool cubeVisible = true;
bool keyPressed = false;
bool display = false;
bool xNeg = false;
bool yNeg = false;
bool xStop = false;
bool yStop = false;
int v = 0;

// Vertices coordinates (Goes from -1 to 1)
GLfloat cubeVertices[] = {
    // 0
    -0.5f, -0.5f, 0.5f,  // Lower left front
    1.0f, 0.0f, 0.0f,     // Red color
    0.0f, 0.0f,          // Image coords

    // 1
    -0.5f, 0.5f, 0.5f,   // Upper left front
    0.0f, 1.0f, 0.0f,     // Green color
    0.0f, 0.0f,          // Image coords

    // 2
    0.5f, 0.5f, 0.5f,    // Upper right front
    0.0f, 0.0f, 1.0f,     // Blue color
    0.0f, 0.0f,          // Image coords

    // 3
    0.5f, -0.5f, 0.5f,   // Lower right front
    1.0f, 1.0f, 0.0f,     // Yellow color
    0.0f, 0.0f,          // Image coords

    // 4
    -0.5f, -0.5f, -0.5f,  // Lower left back
    0.0f, 1.0f, 1.0f,     // Cyan color
    0.0f, 0.0f,           // Image coords

    // 5
    -0.5f, 0.5f, -0.5f,   // Upper left back
    1.0f, 0.0f, 1.0f,     // Magenta color
    0.0f, 0.0f,           // Image coords

    // 6
    0.5f, 0.5f, -0.5f,    // Upper right back
    0.5f, 0.5f, 0.5f,     // Gray color
    0.0f, 0.0f,           // Image coords

    // 7
    0.5f, -0.5f, -0.5f,   // Lower right back
    1.0f, 0.5f, 0.0f,     // Orange color
    0.0f, 0.0f            // Image coords
};

GLuint indices[] = {
    // Front face
    0, 1, 2,
    0, 2, 3,

    // Left face
    4, 5, 1,
    4, 1, 0,

    // Right face
    3, 2, 6,
    3, 6, 7,

    // Bottom face
    0, 3, 7,
    0, 7, 4,

    // Top face
    1, 5, 6,
    1, 6, 2,

    // Back face
    5, 4, 7,
    5, 7, 6
};

GLfloat* RenderImGui(GLFWwindow* window, bool& cubeMove, bool& cubeVisible, bool& display, bool& xNeg, bool& yNeg, bool& xStop, bool& yStop, int& v, GLfloat cubeVertices[]) {
    // Set up the main ImGui window
    ImGui::SetNextWindowSize(ImVec2(1000, 800)); // Set the window size to accommodate all buttons
    ImGui::SetNextWindowPos(ImVec2(10, 10)); // Set the window position
    ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

    // Set text size
    ImGui::SetWindowFontScale(1.2); // Set text scale

    if (ImGui::Button("Close Application", ImVec2(150, 50))) {
        glfwSetWindowShouldClose(window, true);
    }

    // Move to the right side of the window
    ImGui::SameLine(825);

    if (ImGui::Button(cubeMove ? "Stop Spinning" : "Start Spinning", ImVec2(150, 50))) {
        cubeMove = !cubeMove;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 1 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 1;
    }

    ImGui::SameLine(825);

    if (ImGui::Button(cubeVisible ? "Hide Cube" : "Show Cube", ImVec2(150, 50))) {
        cubeVisible = !cubeVisible;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 2 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 2;
    }

    ImGui::SameLine(825);

    if (ImGui::Button("Reverse X", ImVec2(150, 50))) {
        xNeg = !xNeg;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 3 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 3;
    }

    ImGui::SameLine(825);

    if (ImGui::Button(xStop ? "Start X" : "Stop X", ImVec2(150, 50))) {
        xStop = !xStop;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 4 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 4;
    }

    ImGui::SameLine(825);

    if (ImGui::Button("Reverse Y", ImVec2(150, 50))) {
        yNeg = !yNeg;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 5 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 5;
    }

    ImGui::SameLine(825);

    if (ImGui::Button(yStop ? "Start Y" : "Stop Y", ImVec2(150, 50))) {
        yStop = !yStop;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 6 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 6;
    }

    ImGui::SameLine(825);

    if (ImGui::Button(cubeVisible ? "Cube Visible" : "Cube Invisible", ImVec2(150, 50))) {
        cubeVisible = !cubeVisible;
    }

    // Move to the next line
    ImGui::NewLine();

    if (ImGui::Button("Vertice 7 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 7;
    }

    ImGui::NewLine();

    if (ImGui::Button("Vertice 8 Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = 8;
    }

    ImGui::NewLine();

    if(ImGui::Button("All Vertices Color", ImVec2(150, 50))) {
        cubeVisible = false;
        display = true;
        v = -1;
    }

    // Here you can update the color of the vertices based on the value of `v`
    if (display) {
            ImGui::NewLine();

            // RGB scale selector
            static ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Initial color
            ImGui::ColorEdit3("RGB", (float*)&color); // Color picker for RGB

            // Display the selected color
            ImGui::Text("Selected Color:");
            ImGui::SameLine();
            ImGui::ColorButton("##ColorButton", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(50, 25));
            ImGui::SameLine();
            if (ImGui::Button("Enter", ImVec2(50, 25))) {
                float red = color.x;
                float green = color.y;
                float blue = color.z;
                // Display the selected color
                ImGui::Text("Selected Color:");
                ImGui::SameLine();
                ImGui::ColorButton("##ColorButton", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(50, 25));

                if (v == -1) {
                    for(int i = 0; i < 8; i++) {
                        cubeVertices[(8 * (i) + 3)] = red;
                        cubeVertices[(8 * (i) + 4)] = green;
                        cubeVertices[(8 * (i) + 5)] = blue;
                    }
                }
                else {
                    cubeVertices[(8 * (v-1) + 3)] = red;
                    cubeVertices[(8 * (v-1) + 4)] = green;
                    cubeVertices[(8 * (v-1) + 5)] = blue;
                }
                
                cubeVisible = true;
                display = false;
            }
        }

    ImGui::End();

    return cubeVertices;
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Tell GLFW the version being used + Core profile is being used (Only modern functions)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Telling GLFW that a mac is being used
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Making GLFW window
    GLFWwindow* window = glfwCreateWindow(1000, 800, "RGB Square", NULL, NULL);
    // Making sure window can be created
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Putting the window into the current context
    glfwMakeContextCurrent(window);

    // Loading opengl configurations
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell opengl the area of the window
    // Bottom left --> top right
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("SpinningCube/default.vert", "SpinningCube/default.frag");

    // Generates Vertex Array Object and binds it
    VAO vao;
    vao.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO vbo(cubeVertices, sizeof(cubeVertices));
    // Generates Element Buffer Object and links it to indices
    EBO ebo(indices, sizeof(indices));

    // Links VBO to VAO
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0); // Position attribute
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Color attribute
    //vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Image Coords attribute

    // Unbind all to prevent accidentally modifying them
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // Creating rotation variables
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    double prevTime = glfwGetTime();

    // Enables z-buffer so OpenGL knows which triangle texture goes on top of another
    glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui
        // RenderImGui will now update and return the new cubeVertices
        RenderImGui(window, cubeMove, cubeVisible, display, xNeg, yNeg, xStop, yStop, v, cubeVertices);

        // Update the VBO with new vertex data
        vbo.BufferData(cubeVertices, sizeof(cubeVertices));

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader program
        shaderProgram.Activate();

        // Update rotation if cube is spinning
        if (cubeMove) {
            double currentTime = glfwGetTime();
            if (currentTime - prevTime >= 1 / 60) {
                if (!xStop) {
                    if (xNeg) {
                        rotationX -= 0.5f;
                    } else {
                        rotationX += 0.5f;
                    }
                }
                if (!yStop) {
                    if (yNeg) {
                        rotationY -= 0.5f;
                    } else {
                        rotationY += 0.5f;
                    }
                }
                prevTime = currentTime;
            }
        }

        // Initialize matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Assign transformations
        model = glm::rotate(model, glm::radians(rotationY), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotationX), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        // Pass matrices to vertex shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Bind VAO
        vao.Bind();
        if (cubeVisible) {
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        }

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
