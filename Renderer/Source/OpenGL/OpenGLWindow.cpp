#include "OpenGLWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern "C"
{
    static void OnWindowResized(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}

OpenGLWindow::OpenGLWindow(const std::string& name, const uint32_t width, const uint32_t height) : Window(name, width, height),
	m_pGLFWWindow(nullptr)
{}

OpenGLWindow::~OpenGLWindow()
{}

void OpenGLWindow::Initialize()
{
    m_pGLFWWindow = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), NULL, NULL);
    if (m_pGLFWWindow == nullptr)
    {
        std::cout << "ERROR: Failed to create GLFW window " << m_Name << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_pGLFWWindow);

    // Initialise glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, m_Width, m_Height);

    glfwSetFramebufferSizeCallback(m_pGLFWWindow, OnWindowResized);
}

bool OpenGLWindow::Update(const float deltaTime)
{
    bool bShouldUpdate = false;
    bShouldUpdate = (!glfwWindowShouldClose(m_pGLFWWindow));
    if (bShouldUpdate)
    {
        glfwSwapBuffers(m_pGLFWWindow);
        glfwPollEvents();
    }
    return bShouldUpdate;
}

void OpenGLWindow::Deinitialize()
{

}