#pragma once
#ifndef _TERRAINEVENT_H_
#define _TERRAINEVENT_H_

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
		void loadTerrEvMeshes(TextureLoader* texLoader);
		void addAmbush(vec3 loc, mat4 rot);
		void addMerchantStand(vec3 loc, mat4 rot);
		void addRandomDuder(vec3 loc, mat4 rot);
		void addStartCity(vec3 loc);
        void addEndCity(vec3 loc);
		void drawTerrainEvents(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord, double ltime);
		void lowerBridge();
		void reset(); //resets the class erasing all events

	private:
		Materials *matSetter;
		FrustumCull *fCuller;
		float bridgeAng;
		float ropeScale;
		bool moveBridge;
		vec3 bridgeLoc, ropeLoc;
		void addEventMesh(const string filename, bool noNorms);
		void addEventItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void addEventCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void enableTextureBuffer(GLint h_aTexCoord, GLuint texBuf,int id);
		void enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf);
		void disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);
		int getRandInt(int limit);
		float getRandFloat(float limit);
		void setBridge(double ltime);
		int findTex(int num);

};
#endif
