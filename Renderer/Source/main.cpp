#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern "C"
{
    static void GLFWErrorCallback(int error, const char* description);
}

int main(int argc, char** argv)
{
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

    // TODO: Have renderer application that has their calls here now that the Renderer has been initialised?

    glfwTerminate();

	return 0;
}

extern "C"
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }
}