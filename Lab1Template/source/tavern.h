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
#include "mercenary.h"

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
		void drawTavern(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);
	private:
		void loadBufferData();
		void createTable1(glm::vec3 initLoc, float ang);
		void createPillar(glm::vec3 initLoc);
		void addTavernMesh(const string filename, bool noNorms);
		void addTavernItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void enableTextureBuffer(int index, GLint h_aTexCoord, GLuint texBuf);
		void enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf);
		void disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);
};

#endif
