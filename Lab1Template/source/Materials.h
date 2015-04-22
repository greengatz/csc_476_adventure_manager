#pragma once
#ifndef _MATERIALS_H_

#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Materials
{
public:
	GLuint *pid;
	GLint *h_ka;
	GLint *h_kd;
	GLint *h_ks;
	GLint *h_s;
	Materials();
	// virtual ~Materials();
	void init(GLuint *newPid, GLint *newh_ka, GLint *newh_kd, GLint *newh_ks, GLint *newh_s); 
	void setMaterial(int i);
};

#endif