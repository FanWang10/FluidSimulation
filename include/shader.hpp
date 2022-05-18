#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
	public:
    		unsigned int program;

    		Shader(const char * vertexPath, const char * fragmentPath)
    		{
        		std::string vertexCode, fragmentCode;
        		std::ifstream vShaderFile, fShaderFile;

        		// ensure ifstream objects can throw exceptions:
            		vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            		fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
          		try
          		{
				// open files
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				// create string stream
				std::stringstream vShaderStream, fShaderStream;
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close the file
				vShaderFile.close();
				fShaderFile.close();
				// get the string
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
          		}
          		catch(std::ifstream::failure& e)
          		{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
          		}
          		// get char string ver of code
          		const char* vShaderCode = vertexCode.c_str();
          		const char* fShaderCode = fragmentCode.c_str();

			// Create Shader Object
			unsigned int vertex, fragment;

			// Vertex Shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);
			// ---------------------
			checkCompileErrors(vertex, "SHADER");
			// --------------------

			// Fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			// ---------------------
			checkCompileErrors(fragment, "SHADER");
			// --------------------

			// Program
			program = glCreateProgram();
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);
			// ---------------------
			checkCompileErrors(program, "PROGRAM");
			// --------------------

			glDeleteShader(vertex);
			glDeleteShader(fragment);
	    	}	

		void use()
		{
			glUseProgram(program);
		}

		void setBool(const std::string &name, bool value) const
		{
			glUniform1i(
				glGetUniformLocation(program, name.c_str()),
				(int) value
			);
		}

		void setInt(const std::string &name, int value) const
		{
			glUniform1i(
				glGetUniformLocation(program, name.c_str()),
				value
			);
		}

    		void setFloat(const std::string &name, float value) const
    		{
			glUniform1f(
				glGetUniformLocation(program, name.c_str()),
				value
			);
    		}

		void destroy()
		{
			glDeleteProgram(program);
		}

		unsigned int getID()
		{
			return program;
		}

    	private:
        	void checkCompileErrors(unsigned int id, std::string type)
        	{
			int success;
			char infoLog[1024];
			if(type != "PROGRAM")
			{
				// get the value of parameter: GL_COMPILE_STATUS
				glGetShaderiv(id, GL_COMPILE_STATUS, &success);
				if(!success)
				{
					glGetShaderInfoLog(id, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
				}				
			}
			else
    			{
        			// get the value of parameter: GL_LINK_STATUS
        			glGetProgramiv(id, GL_LINK_STATUS, &success);
        			if(!success)
        			{
					glGetProgramInfoLog(id, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINK_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
        			}
    			}	
        	}
};

#endif
