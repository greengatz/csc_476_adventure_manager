/**
 * Wagon.h - Header file for Wagon object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _WAGON_H_
#define _WAGON_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include "RenderingHelper.h"
#include "Terrain.h"
#include "manager.h"
#include "menu.h"
#include "SoundPlayer.h"
#include "Materials.h"
#include "Wagon.h"
#include "pngTexture.h"
#include "Bitmap.h"
#include "stb_image.h"

class Wagon
{
public:
	Wagon();
	virtual ~Wagon();

//	void init(TextureLoader* texLoader, Terrain* aTerrain, Menu* aMenu, bool* gP, Manager* mgr, ProjectMeshes *newData, SoundPlayer* audio);
	void drawMercs(GLint h_ModelMatrix, GLint h_vertPos, 
            GLint h_vertNor, GLint h_aTexCoord, GLint h_boneFlag,
            GLint h_boneIds, GLint h_boneWeights,
            GLint h_boneTransforms, float time);
	void init(TextureLoader* texLoader, Terrain* aTerrain, Menu* aMenu, bool* gP, float* fF, Manager* mgr, ProjectMeshes *newData, SoundPlayer* audio);
	void draw(RenderingHelper* modelTrans, Camera* camera, glm::vec3 wagonPos);
	void setPosition(float x, float y, float z);
	glm::vec3 getPosition();
	void setScale(glm::vec3 aScale);
	void setRotation(float aRotation);
	void resetWagon();
	void startWagon();
	void updateWagon(float globalTime);
	bool hasStarted();
	void setTimeStamp(float newTime);

private:
	GLuint pid;
	GLint h_vertPos;
	GLint h_vertNor;
	GLint h_aTexCoord;

	//Handles to the shader data
	GLint h_uTexUnit;
	GLint h_ProjMatrix;
	GLint h_ViewMatrix;
	GLint h_ModelMatrix;

	//Material Color
	GLint h_ka;
	GLint h_kd;
	GLint h_ks;
	GLint h_s;
	GLint lightPosID;

	//Shadow data
	GLuint pidShadow;
  	GLint h_vertPosShadow;
  	GLint h_vertNorShadow;

	GLint h_ViewMatrixShadow;
	GLint h_ModelMatrixShadow;
	GLint h_ProjMatrixShadow;
	GLint h_ProjMatrixShadMat;
	GLint lightPosIDShadow;

	std::vector<tinyobj::shape_t> shapes;
	GLuint posBufID;
	GLuint norBufID;
	GLuint texBufID;
	GLuint indBufID;
	glm::vec3 position;
	glm::vec3 scale;
	float rotate;
	float startTime;
	Terrain* terrain;
	Menu* menu;
	Manager* manager;
	ProjectMeshes *meshData;
	float* fastForward;
	bool* gamePaused;
	float deltaTime;
	bool wagonStart;
	float velocity;
    SoundPlayer* soundSys;
	glm::vec3 direction;
	glm::vec3 nextPoint;
	glm::vec3 orientation;

	Materials mat;

	tdogl::pngTexture* merchantSlayerMenu;
	tdogl::pngTexture* merchantMenu;
	tdogl::pngTexture* sicknessMenu;
	tdogl::pngTexture* buyMercMenu;
	tdogl::pngTexture* banditsMenu;
	tdogl::pngTexture* beggarMenu;

};

#endif
