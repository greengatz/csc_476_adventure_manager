#ifndef MANAGER_H
#define MANAGER_H

#include "GLSL.h"
// #include "bounding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Camera.h"
#include <iostream>
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3dcontainer.h"
#include "obj3d.h"

using namespace std;
using namespace glm;

class Manager
{
	public:
		// GLint *h_vertPos, *h_norPos; //buffers to draw to
		// vector<Obj3dContainer> tavernMeshes; //obj meshes holding image data
		// vector<Obj3d> tavernItems; //data for each obj in the tavern - position, rotation, etc
		Manager(string name, Camera camera);
		// void loadTavernMeshes();
		// void drawTavern(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor);

		double gold;
		int food;
		int beer;
		int mercs;

	private:
		Camera camera;
		vec3 position;
		string name;

		void reportStats();
};

#endif
