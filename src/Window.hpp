#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
    private:
        int width;
        int height;
        GLFWwindow * window;
        void Initialize();
        void Terminate();
    public:
        Window();
        Window(int width, int height, const char* windowName);
        void OnExecute();
};


#endif