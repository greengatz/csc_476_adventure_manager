#ifndef TAVERN_H
#define TAVERN_H

#include "GLSL.h"
//#include "bounding.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <Importer.hpp>
//#include <scene.h>
//#include <postprocess.h>
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
#include "charDae.h"
#include "ProjectMeshes.h"

using namespace std;
using namespace glm;

class Tavern
{
	public:
		
		// GLint *h_vertPos, *h_norPos; //buffers to draw to
		// vector<Obj3dContainer> tavernMeshes; //obj meshes holding image data
		vector<Obj3d> tavernItems; //data for each obj in the tavern - position, rotation, etc
		vector<Mercenary> tavernCharacters;
		vec3 getDoorLoc();
		vec3 getBeerLoc();
		vec3 getFoodLoc();
		Tavern();
		void spawn3NewMercs();
		void init(Materials *newMatSetter, FrustumCull *newCuller, ProjectMeshes *newData, GLint* normalToggleID);
		void loadTavernMeshes(TextureLoader* texLoader);
		void showMercsForSale();
		void newEmblem();
        //void buyMercenary(int mercenaryID, Manager* purchaser);
		void drawTavern(GLint h_ModelMatrix, GLint h_vertPos, 
                    GLint h_vertNor, GLint h_aTexCoord, double ltime,
                    GLint h_boneFlag, GLint h_boneIds, 
                    GLint h_boneWeights, GLint h_boneTransforms);
		void drawTavernMercs(GLint h_ModelMatrix, GLint h_vertPos, 
                    GLint h_vertNor, GLint h_aTexCoord, double ltime,
                    GLint h_boneFlag, GLint h_boneIds, 
                    GLint h_boneWeights, GLint h_boneTransforms, GLint h_texFlag,
                    GLint h_boneIds2, GLint h_boneWeights2);


	private:
		float curTime;
		Materials *matSetter;
		FrustumCull *fCuller;
		ProjectMeshes *meshData;
		vec3 doorLoc;
		vec3 beerLoc;
		vec3 foodLoc;
      GLint* normToggleID;
		int wallEmblem;
		float getRandFloat(float limit);
		int getRandInt(int limit);
		void loadBufferData(TextureLoader* texLoader);
		void createTable1(glm::vec3 initLoc, float ang);
		void createPillar(glm::vec3 initLoc, bool north);
		void createFirePlace(glm::vec3 init);
		void createEmblems();
		// void addTavernMesh(const string filename, bool noNorms);
		void addTavernItem(int index, int vectMesh, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void addTavernCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot);
		void enableTextureBuffer(GLint h_aTexCoord, GLuint texBuf,int id);
		void enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf);
		void disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);
		void applyTurkeySpin(double ltime);
      void enableNormalBuffer(GLint h_aTexCoord, GLuint texBuf, int id);

        CharDae sam;
        CharDae *fellowTraveler, *fellowTraveler2;
};

#endif
