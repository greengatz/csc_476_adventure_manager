#ifndef TAVERN_H
#define TAVERN_H

#include "GLSL.h"
// #include "bounding.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3dcontainer.h"
#include "obj3d.h"

using namespace std;
using namespace glm;

class Tavern
{
	public:
		// GLint *h_vertPos, *h_norPos; //buffers to draw to
		vector<Obj3dContainer> tavernMeshes; //obj meshes holding image data
		vector<Obj3d> tavernItems; //data for each obj in the tavern - position, rotation, etc
		Tavern();
		void loadTavernMeshes();
		void drawTavern(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor);
	private:
		void addTavernMesh(const string filename);
		void addTavernItem(int index, glm::vec3 scale, glm::vec3 trans);
		void enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf);
		void disableBuff(GLint h_vertPos, GLint h_vertNor);
};

#endif