#include "Shader.hpp"
#include <string>
#include <fstream>
#include <sstream>

void checkForCompileError(unsigned int shader, const char* shaderType)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        // print error report
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderType << 
        "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void checkForLinkError(unsigned int program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << 
        infoLog << std::endl;
    }
}

// read in shader code
void readInShader(const char* shaderPath, std::string & code)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open file
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    } 
}

unsigned int LoadShaders(const char* vertexShaderSrc, const char* fragShaderSrc)
{


    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexCode;
    readInShader(vertexShaderSrc, vertexCode);
    const char* vertexCodeCharPtr = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vertexCodeCharPtr, NULL);
    glCompileShader(vertexShader);
    // checking for compile errors
    checkForCompileError(vertexShader, "VERTEX");

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentCode;
    readInShader(fragShaderSrc, fragmentCode);
    const char* fragmentCodeCharPtr = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentCodeCharPtr, NULL);
    glCompileShader(fragmentShader);
    // checking for comiple errors
    checkForCompileError(fragmentShader, "FRAGMENT");

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for error again
    checkForLinkError(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;

}