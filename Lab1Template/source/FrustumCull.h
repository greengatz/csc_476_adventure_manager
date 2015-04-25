#pragma once
#ifndef _FRUSTUMCULL_H_
#define _FRUSTUMCULL_H_

#include <cstdlib>
#include <iostream>
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "obj3d.h"

using namespace glm;

class FrustumCull
{
public:
	FrustumCull();
	virtual ~FrustumCull();
	void init();
	bool checkCull(Obj3d target);
	void toggleMode();
	void setProjMat(mat4 newProjMat);
private:
	mat4 curProjMat;
	bool toggled; //true means cull
	bool checkPlane(const float** matIter, vec4 vert, int row, int sign, vec3 center, float radius);
};

#endif