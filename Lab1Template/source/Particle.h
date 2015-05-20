#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

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
	Particle();
	void init(int staggerTime, bool isSmoke);
	void rebirth(float curTime);
	void update(float ltime, float h);
	// void draw(glm::vec3 loc, GLint h_scale, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos); //soon to be deleted
	void drawFirePlace(glm::vec3 loc, GLint h_scale, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos);
	void drawTorch(glm::vec3 loc, GLint h_scale, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos);
	bool isSmokey();
private:
	int stagger;
	bool smoke;
	float mass;
	float damp;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifespan;
	float endLife;
	float scale;
	glm::vec4 col;
	float randFloat(float low, float high);
};

#endif