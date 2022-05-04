#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

unsigned int LoadShaders(const char* vertexShaderSrc, const char* fragShaderSrc);

#endif