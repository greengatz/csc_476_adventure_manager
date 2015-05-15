#pragma once
#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "GLSL.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Particle
{
public:
	Particle(glm::vec3 pos);
	void init();
	void rebirth();
	void update();
private:
	float mass;
	float damp;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifespan;
	float endLife;
	float scale;
	glm::vec4 col;
};

#endif