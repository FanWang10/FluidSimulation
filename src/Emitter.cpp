#include "Emitter.hpp"
#include <random>
#define P_SIZE 0.1f


float generateRandom(float min, float max)
{
    return ((float) rand() / (RAND_MAX)) * (max - min) + min;
}

void pushPoint(float x, float y, float z, std::vector< float > & vertices)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void pushSquare(Particle p, std::vector< float > & vertices)
{
    // push triangle 1
    pushPoint(p.x - (P_SIZE / 2.0f), p.y + (P_SIZE / 2.0f), p.z, vertices);

    pushPoint(p.x + (P_SIZE / 2.0f), p.y + (P_SIZE / 2.0f), p.z, vertices);

    pushPoint(p.x + (P_SIZE / 2.0f), p.y - (P_SIZE / 2.0f), p.z, vertices);
    // push triangle 2

    pushPoint(p.x - (P_SIZE / 2.0f), p.y + (P_SIZE / 2.0f), p.z, vertices);

    pushPoint(p.x - (P_SIZE / 2.0f), p.y - (P_SIZE / 2.0f), p.z, vertices);

    pushPoint(p.x + (P_SIZE / 2.0f), p.y - (P_SIZE / 2.0f), p.z, vertices);
}

Emitter::Emitter(int size)
{
    // generate #size particles into vector::particles
    for(int i = 0; i < size; i++)
    {
        Particle p{
            generateRandom(-1.0f, 1.0f),
            generateRandom(-1.0f, 1.0f),
            generateRandom(-1.0f, 1.0f),
            generateRandom(0.0f, 1.0f)
        };
        particles.push_back(p);
    }
}

void Emitter::update(float time)
{
    // assume move toward x diretion
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i].x += time * 0.01f;
    }
}

unsigned int Emitter::draw()
{
    for(int i = 0; i < particles.size(); i++)
    {
        pushSquare(particles[i], vertices);
    }

    return configureVertexAttribute(&vertices[0], vertices.size());
}

