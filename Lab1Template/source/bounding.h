#ifndef BOUNDING_H
#define BOUNDING_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr


using namespace std;
using namespace glm;

class Bounding
{
	public:
		virtual void createBounds(vec2 xSize, vec2 ySize, vec2 zSize) {}
		virtual void createBounds(vector<tinyobj::shape_t> objShape) {}
		virtual bool checkCollision(float cam[], vec3 scale, vec3 trans) {return false;}
};

class BoundingBox : public Bounding
{
	public:
		void createBounds(vector<tinyobj::shape_t> objShape);
		void createBounds(vec2 xSize, vec2 ySize, vec2 zSize);
		bool checkCollision(float cam[], vec3 scale, vec3 trans);
		void calcSphere(vec3 scale, vec3 pos);
		void calcSphere(vec3 newCenter, float newRad);
		float minX, maxX;
		float minY, maxY;
		float minZ, maxZ;
		vec3 center;
		float radius;
	private:

};

#endif