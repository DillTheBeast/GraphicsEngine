//g++ -std=c++17 \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/Main.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/src/glad.c \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/EBO.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/VAO.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/VBO.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/shaderClass.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/Camera.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/imgui.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/imgui_demo.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/imgui_draw.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/imgui_widgets.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/imgui_tables.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/backends/imgui_impl_opengl3.cpp \
    /Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/backends/imgui_impl_glfw.cpp \
    -o main \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/glm \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui \
    -I/Users/dillonmaltese/Documents/git/GraphicsEngine/MovingCube/include/imgui/backends \
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
#include "Camera.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

// bool cubeMove = true;
// bool cubeVisible = true;
// bool keyPressed = false;
// bool display = false;
// bool xNeg = false;
// bool yNeg = false;
// bool xStop = false;
// bool yStop = false;
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

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f; 
float lastFrame = 0.0f;

// Callback function for mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - xpos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback function for mouse scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
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

    // Set callbacks for mouse input
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Tell opengl the area of the window
    // Bottom left --> top right
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("MovingCube/default.vert", "MovingCube/default.frag");

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

    // Enables z-buffer so OpenGL knows which triangle texture goes on top of another
    glEnable(GL_DEPTH_TEST);

    //Main loop
    while (!glfwWindowShouldClose(window)) {
        // Time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        // Camera setup
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        shaderProgram.SetMat4("view", view);
        shaderProgram.SetMat4("projection", projection);

        // Bind the VAO so OpenGL knows to use it
        vao.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Handle GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shaderProgram.Delete();

    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}