#pragma once
#ifndef _FIRESYSTEM_H_
#define _FIRESYSTEM_H_

#include "GLSL.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Particle.h"

class FireSystem
{
public:
	FireSystem();
	void init();
	void draw();
private:
	vector<Particle> torch;
	vector<Particle> firePlace;
	GLuint posBufID;
	GLuint texBufID;
	GLuint indBufID;
};

#endif