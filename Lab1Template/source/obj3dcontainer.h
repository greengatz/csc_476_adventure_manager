#ifndef OBJ3DCONTAINER_H
#define OBJ3DCONTAINER_H

#include "GLSL.h"
#include <stdio.h>
#include <iostream>
// #include "bounding.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

// #include <glew.h>
// #include <glfw3.h>

using namespace std;
using namespace glm;

class Obj3dContainer
{
	public:
		Obj3dContainer();
		Obj3dContainer(const string name);
		vector<tinyobj::shape_t> shape;
		vector<tinyobj::material_t> material;
		GLuint posBuf, norBuf, indBuf;
		void initBuffers(bool noNorms);
		int getIndices();
		void loadIntoTinyOBJ(const string name);

};

#endif