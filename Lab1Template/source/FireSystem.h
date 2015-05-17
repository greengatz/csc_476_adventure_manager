#pragma once
#ifndef _FIRESYSTEM_H_
#define _FIRESYSTEM_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Particle.h"
#include "Camera.h"

using namespace std;

class FireSystem
{
public:
	FireSystem();
	void init(TextureLoader *texLoader);
	void draw(Camera *cam, glm::mat4 viewMat);
	void toggle();
private:
	vector<Particle> torch; //plan: 1 torch particle system draw 4 times
	vector<Particle> firePlace;

	GLuint pid;

	//handles
	GLint h_vertPos;
	GLint h_vertTexCoor;
	GLint h_scale;
	GLint h_tex;
	GLint h_color;
	GLint h_ProjMat;
	GLint h_ModelMat;
	GLint h_ViewMat;

	GLuint posBufID;
	GLuint texBufID;
	GLuint indBufID;
	std::vector<float> posBuf;
	std::vector<float> texBuf;
	std::vector<unsigned int> indBuf;

	float ltime;
	float timeIncr; //amount to increment time, may use dtime instead

	bool on;
	void loadBuffers();
};

#endif