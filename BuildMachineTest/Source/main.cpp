#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ExampleDefines.h"

static void GLFWErrorCallback(int error, const char* description);
void OnWindowResized(GLFWwindow* window, int width, int height);

int main(int argc, char** argv)
{
#if OPENGL_WINDOW
    glfwSetErrorCallback(GLFWErrorCallback);

    // Initialise GLFW
    if (!glfwInit())
    {
        // Initialization failed
        std::cout << "ERROR: GLFW init failed" << std::endl;
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if PLATFORM_MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the window
    constexpr int width{ 800 };
    constexpr int height{ 600 };
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell OpenGL the size of the window we want to render
    glViewport(0, 0, width, height);

    // Callback to handle windows being resized
    glfwSetFramebufferSizeCallback(window, OnWindowResized);

    constexpr float buildMachineTimedTest{ 5.0f }; // How long in seconds until the window should automatically close.
    float currentTime = 0.0f;

    // Render loop
    // TODO: Make this multithreaded?
    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        const float time{ (float)glfwGetTime() };
        const float deltaTime{ time - lastTime };
        lastTime = time;

        currentTime += deltaTime;
        if (currentTime >= buildMachineTimedTest)
        {
            std::cout << buildMachineTimedTest << " seconds have passed, terminating executable now!" << std::endl;
            break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

#endif

	return 0;
}

static void GLFWErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void OnWindowResized(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}