#ifndef TAVERN_H
#define TAVERN_H

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

class Tavern
{
	public:
		
		// GLint *h_vertPos, *h_norPos; //buffers to draw to
		vector<Obj3dContainer> tavernMeshes; //obj meshes holding image data
		vector<Obj3d> tavernItems; //data for each obj in the tavern - position, rotation, etc
		vector<Mercenary> tavernCharacters;
		vec3 getDoorLoc();
		vec3 getBeerLoc();
		vec3 getFoodLoc();
		Tavern();
		void init(Materials *newMatSetter, FrustumCull *newCuller);
		void loadTavernMeshes(TextureLoader* texLoader);
		void showMercsForSale();
        //void buyMercenary(int mercenaryID, Manager* purchaser);
		void drawTavern(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord, double ltime);

	private:
		float curTime;
		Materials *matSetter;
		FrustumCull *fCuller;
		vec3 doorLoc;
		vec3 beerLoc;
		vec3 foodLoc;
		float getRandFloat(float limit);
		int getRandInt(int limit);
		void loadBufferData(TextureLoader* texLoader);
		void createTable1(glm::vec3 initLoc, float ang);
		void createPillar(glm::vec3 initLoc);
		void createFirePlace(glm::vec3 init);
		void addTavernMesh(const string filename, bool noNorms);
		void addTavernItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void addTavernCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void enableTextureBuffer(GLint h_aTexCoord, GLuint texBuf,int id);
		void enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf);
		void disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);
		void applyTurkeySpin(double ltime);
};

#endif
