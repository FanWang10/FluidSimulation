#ifndef EMITTER_HPP
#define EMITTER_HPP
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "VertexAttrib.hpp"

struct Particle
{
    float x, y, z, lifetime;
};


class Emitter
{
    public:
        Emitter(int size);
        void update(float time);
        unsigned int  draw();
        int sizeOfPoints()
        {
            return vertices.size() / 3;
        }

    private:
        std::vector< Particle > particles;
        std::vector< float > vertices;
};

#endif