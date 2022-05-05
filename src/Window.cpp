#include "Window.hpp"
#include "Shader.hpp"
#include "VertexAttrib.hpp"
#include <iostream>
#include <math.h>
#include "Emitter.hpp"


void InitGLFW()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

Window::Window()
{
    InitGLFW();
    width = 500;
    height = 500;
    window = glfwCreateWindow(width, height, "Window", NULL, NULL);
}

Window::Window(int w, int h, const char* windowName)
{
    InitGLFW();
    width = w;
    height = h;
    window = glfwCreateWindow(width, height, windowName, NULL, NULL);
}

void Window::Initialize()
{
   if(window == NULL)
   {
       std::cout << "Failed to create GLFW window" << std::endl;
       glfwTerminate();
   }
   else
   {
       glfwMakeContextCurrent(window);
       if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
       {
           std::cout << "Failed to intialize GLAD" << std::endl;
       }else
       {
           framebuffer_size_callback(window, 800, 600);
           glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
       }
   }
}

void Window::Terminate()
{
    glfwTerminate();
}

void Window::OnExecute()
{
    Initialize();
    unsigned int shaderProgram = LoadShaders("./shader/vertexShader.shader", "./shader/fragmentShader.shader");

    // the square vertices
    float vertices[] = 
    {
        0.4f, 0.1f, 0.0f,
        0.5f, 0.1f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.4f, 0.1f, 0.0f,
        0.4f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,

        -0.4f, -0.1f, 0.0f,
        -0.5f, -0.1f, 0.0f,
        -0.5f, 0.0f, 0.0f,
        -0.4f, -0.1f, 0.0f,
        -0.4f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f,

    };


    unsigned int VAO = configureVertexAttribute(vertices, sizeof(vertices));

    while(!glfwWindowShouldClose(window))
	{
        glUseProgram(shaderProgram);

		// draw background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        /* Update The Vertices*/        
        VAO = configureVertexAttribute(vertices, sizeof(vertices));

        // draw out first triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/(sizeof(float) * 3));

		glfwSwapBuffers(window);
		glfwPollEvents();
		processInput(window);
	}
    Terminate();

}