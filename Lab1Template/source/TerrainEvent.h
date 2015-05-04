#ifndef TERRAINEVENT_H
#define TERRAINEVENT_H

#include "GLSL.h"
//#include "bounding.h"
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
#include "TextureLoader.h"
#include "Materials.h"
#include "FrustumCull.h"

using namespace std;
using namespace glm;

class TerrainEvent
{
	public:
		vector<Obj3dContainer> meshes; //obj meshes holding image data, soon will be joined to load all of these in 1 spot
		vector<Obj3d> eventItems; //data for each obj in the tavern - position, rotation, etc
		vector<Mercenary> eventCharacters;
		TerrainEvent();
		void init(Materials *newMatSetter, FrustumCull *newCuller);
		void loadTavernMeshes(TextureLoader* texLoader);
		void drawTerrainEvents(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);

	private:
		Materials *matSetter;
		FrustumCull *fCuller;
		void loadBufferData(TextureLoader* texLoader);
		void addEventMesh(const string filename, bool noNorms);
		void addEventItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void addEventCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void enableTextureBuffer(GLint h_aTexCoord, GLuint texBuf,int id);
		void enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf);
		void disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);
};

#endif
