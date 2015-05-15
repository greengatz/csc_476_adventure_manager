#include "Particle.h"

float randFloat(float low, float high)
{
	float num = rand() / (float)RAND_MAX;
	return (1.0f - num) * low + num * high;
}

Particle::Particle(glm::vec3 pos)
{
	mass = 0.01;
	damp = 0;
	life = 1.0;
	scale = 1.0;
}

void Particle::init()
{
	rebirth();
}

void Particle::rebirth()
{
	damp = randFloat(0.0f, 0.01f);
	pos.x = randFloat(-0.15f, 0.15f);
	pos.y = randFloat(-0.15f, 0.15f);
	pos.z = randFloat(-0.15f, 0.15f);
	vel.x = randFloat(0.0f, 0.00000001f);
	vel.y = randFloat(0.0f, 0.00000001f);
	vel.z = randFloat(0.0f, 0.00000001f);
	lifespan = randFloat(5.0f, 20.0f);
	endLife = t + lifespan;
	scale = randFloat(0.45f, 0.65f);
	col.x = 1.0f;
	col.y = randFloat(0.25f, 0.65f);
	col.z = 0.0f;
	col.w = randFloat(0.0f, 1.0f);
}

//not sure what h is....
void Particle::update(float ltime, float h)
{

}