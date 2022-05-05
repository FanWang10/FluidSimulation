#include "Emitter.hpp"
#include <random>
#define P_SIZE 0.1f;

float generateRandom(float min, float max)
{
    return ((float) rand() / (RAND_MAX)) * (max - min) + min;
}

Emitter::Emitter(int size)
{
    // randomly generate #size particles
    for(int i = 0; i < size; i++)
    {
        // particles
        Particle p{generateRandom(-1.0f, 1.0f),
                    generateRandom(-1.0f, 1.0f),
                    generateRandom(-1.0f, 1.0f),
                    generateRandom(0.0f, 1.0f),
        };
        particles.push_back(p);

        // vertices
        vertices.push_back(p.x - (P_SIZE / 2), p.y + (P_SIZE / 2), 0);
        vertices.push_back(p.x + (P_SIZE / 2), p.y + (P_SIZE / 2), 0);
        vertices.push_back(p.x + (P_SIZE / 2), p.y - (P_SIZE / 2), 0);
        vertices.push_back(p.x - (P_SIZE / 2), p.y + (P_SIZE / 2), 0);
        vertices.push_back(p.x - (P_SIZE / 2), P.y - (P_SIZE / 2), 0);
        vertices.push_back(p.x + (P_SIZE / 2), p.y - (P_SIZE / 2), 0);
    }
}

Emitter::update()
{
    for(int i = 0; i < particles.size(); i++)
    {
        //p.x = p.x + 0.01f;
        particles[i].x = particles[i].x + 0.01f;

    }
}

Emitter::draw()
{
    // give values to vertices

    // VAO = configureVertexAttribute(vertices, sizeof(vertices));
    // glBindArray();
    // glDrawArray
}