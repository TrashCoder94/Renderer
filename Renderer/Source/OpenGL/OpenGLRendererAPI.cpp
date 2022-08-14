#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern "C"
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }
}

OpenGLRendererAPI::OpenGLRendererAPI()
{}

OpenGLRendererAPI::~OpenGLRendererAPI()
{}

void OpenGLRendererAPI::Initialize()
{
    glfwSetErrorCallback(GLFWErrorCallback);

    // Initialise GLFW
    if (!glfwInit())
    {
        // Initialization failed
        std::cout << "ERROR: GLFW init failed" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if PLATFORM_MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void OpenGLRendererAPI::Deinitialize()
{
    glfwTerminate();
}

const float OpenGLRendererAPI::GetTime()
{
    return (float)glfwGetTime();
}