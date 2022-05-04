#include "Window.hpp"
#include "Shader.hpp"
#include <iostream>


const char *vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"
;

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
    unsigned int shaderProgram = LoadShaders(vertexShaderSource, fragmentShaderSource);

    // the square vertices
    float vertices[] = 
    {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    while(!glfwWindowShouldClose(window))
	{
        glUseProgram(shaderProgram);

		// rendering command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        

        //-----------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
		processInput(window);
	}

    Terminate();

}