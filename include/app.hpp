#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <cstring>

class App
{
	private:
		int frame;
		int windowWidth;
		int widthHeight;
		GLFWwindow * window;
		unsigned int renderProgram;
		unsigned int computeProgram[3]{0, 0, 0};
		unsigned int particlePosVAO = 0;
		unsigned int particleBuffer = 0;

		
		void loop();
		void compileShader(unsigned int programID, const char * path, GLenum shaderType);
    	public:
        	App()
        	{
			frame = 0;
        	}

        	App(int f)
        	{
			frame = f;
        	}
		void Initialize(int windowWidth, int windowHeight);
		void InitializeShader();
		void Run(int windowWidth, int windowHeight);
		void Destroy();
	private:
    		int numOfParticle = 20000;
    		float particleRadius = 0.005f;
};	
