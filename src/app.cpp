#include "../include/app.hpp"


void processInput(GLFWwindow * window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


void checkCompileError(unsigned int shaderID, const char * type)
{
	int success;
	char infoLog[1024];
	if(type != "PROGRAM")
	{
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
				<< type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_LINKING::ERROR of type: "
			<< type << "\n" << infoLog << std::endl;
		}
	}
}


void App::Initialize(int windowWidth, int windowHeight)
{
	// Create Window Here
	// glfw is initialized too
	glfwInit();
	window = glfwCreateWindow(windowWidth, windowHeight, "SPH", NULL, NULL);
	// check window
	if(!window)
	{
    		std::cout << "Failed to initialize window!\n";
    		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	auto buffer_size_changed = [] (GLFWwindow * theWindow, int newWidth, int newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	};
	glfwSetFramebufferSizeCallback(window, buffer_size_changed);

	// check glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD \n";
		return;
	}
	
}


void App::loop()
{
	processInput(window);
	
	// ---------------------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// -------------- render --------------
	glUseProgram(renderProgram);
	glDrawArrays(GL_POINTS, 0, numOfParticle);
	// -----------------------------------
	
	// ------------- do simulation --------
	glUseProgram(computeProgram[0]);
	glDispatchCompute(workGroups, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	glUseProgram(computeProgram[1]);
	glDispatchCompute(workGroups, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(computeProgram[2]);
	glDispatchCompute(workGroups, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	// ------------------------------------

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void App::compileShader(unsigned int programID, const char * path, GLenum shaderType)
{
	// 1. read the code
	// 2. compile the code with glCompileShader
	// 3. Attach
	// 4. destroy shader

	// READ THE CODE
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream fileSS;
		fileSS << shaderFile.rdbuf();
		shaderCode = fileSS.str();
		shaderFile.close();
	}catch(std::ifstream::failure & e)
	{
		std::cout << "ERROR::CANNOT OPEN THE FILE\n";
	}
	const char * shaderCodeC = shaderCode.c_str();

	// compile the code
	// 1. generate shader object
	unsigned int shaderID = glCreateShader(shaderType);
	// 2. Assign the code to the shader object
	glShaderSource(shaderID, 1, &shaderCodeC, NULL);
	// 3. Compile the shader
	glCompileShader(shaderID);
	// Attach the code to the program
	glAttachShader(programID, shaderID);

	glDeleteShader(shaderID);

	checkCompileError(shaderID, "SHADER");
}

void App::InitializeShader()
{
	// Create Program
    	renderProgram = glCreateProgram(); 
	// vertex shader
	compileShader(renderProgram, "./shader/vertex.vs", GL_VERTEX_SHADER);	
	// fragment shader
	compileShader(renderProgram, "./shader/fragment.fs", GL_FRAGMENT_SHADER);
	// link program
	glLinkProgram(renderProgram);
	checkCompileError(renderProgram, "PROGRAM");

	// Compute Program
	computeProgram[0] = glCreateProgram();
	// density and pressure shader
	compileShader(computeProgram[0], "./shader/densityPressure.comp", GL_COMPUTE_SHADER);
	// link program
	glLinkProgram(computeProgram[0]);
	checkCompileError(computeProgram[0], "PROGRAM");

	// Compute Program
	computeProgram[1] = glCreateProgram();
	// force shader
	compileShader(computeProgram[1], "./shader/force.comp", GL_COMPUTE_SHADER);
	// link program
	glLinkProgram(computeProgram[1]);
	checkCompileError(computeProgram[1], "PROGRAM");
	
	// compute Program 
	computeProgram[2] = glCreateProgram();
	// integrate acceleration to find new positions
	compileShader(computeProgram[2], "./shader/integrate.comp", GL_COMPUTE_SHADER);
	// link program
	glLinkProgram(computeProgram[2]);
	checkCompileError(computeProgram[2], "PROGRAM");

}

void App::Run(int windowWidth, int windowHeight)
{
 	Initialize(windowWidth, windowHeight);   	
	InitializeShader();

	std::vector<glm::vec2> initPosition(numOfParticle);

	if(true)
	{
		for(auto i = 0, x = 0, y = 0; i < numOfParticle; i++)
		{
			initPosition[i].x = -0.625f + particleRadius * 2 * x;
			initPosition[i].y = 1 - particleRadius * 2 * y;
			x++;
			if(x >= 100)
			{
				x = 0;
				y++;
			}
		}
	}
	else
    	{
		
    	}

	// -------------- ssbo sizes -------------
	ptrdiff_t positionSSBOsize = sizeof(glm::vec2) * numOfParticle;
	ptrdiff_t velocitySSBOsize = sizeof(glm::vec2) * numOfParticle;
	ptrdiff_t forceSSBOsize = sizeof(glm::vec2) * numOfParticle;
	ptrdiff_t densitySSBOsize = sizeof(float) * numOfParticle;
	ptrdiff_t pressureSSBOsize = sizeof(float) * numOfParticle;
	ptrdiff_t fullBufferSSBOsize = positionSSBOsize +
					velocitySSBOsize +
					forceSSBOsize +
					pressureSSBOsize +
					densitySSBOsize;
	// -------------- ssbo offsets ----------
	ptrdiff_t positionSSBOoffset = 0;
	ptrdiff_t velocitySSBOoffset = positionSSBOoffset + positionSSBOsize;
	ptrdiff_t forceSSBOoffset = velocitySSBOoffset + velocitySSBOsize;
	ptrdiff_t densitySSBOoffset = forceSSBOoffset + forceSSBOsize;
	ptrdiff_t pressureSSBOoffset = pressureSSBOoffset + pressureSSBOsize;


    	void * initialData = std::malloc(fullBufferSSBOsize);
    	std::memset(initialData, 0, fullBufferSSBOsize);
    	std::memcpy(initialData, initPosition.data(), positionSSBOsize);
	glGenBuffers(1, &particleBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, fullBufferSSBOsize, initialData, GL_DYNAMIC_STORAGE_BIT);
	std::free(initialData);
	
	// VAO VBO and BUFFER OBJECT
	glGenVertexArrays(1, &particlePosVAO);
	glBindVertexArray(particlePosVAO);

	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ------- other binding -------------
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer, positionSSBOoffset, positionSSBOsize);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, particleBuffer, velocitySSBOoffset, velocitySSBOsize);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, particleBuffer, forceSSBOoffset, forceSSBOsize);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, particleBuffer, densitySSBOoffset, densitySSBOsize);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, particleBuffer, pressureSSBOoffset, pressureSSBOsize);
	// ------- other binding -------------

	glBindVertexArray(particlePosVAO);
	
	while(!glfwWindowShouldClose(window))
	{
		loop();
	}

	Destroy();
}

void App::Destroy()
{
    	glDeleteProgram(renderProgram);
    	glDeleteProgram(computeProgram[0]);
    	glDeleteProgram(computeProgram[1]);
    	glDeleteProgram(computeProgram[2]);
    	if(window != nullptr)
    	{
		glfwDestroyWindow(window);
    	}
	glfwTerminate();
}
