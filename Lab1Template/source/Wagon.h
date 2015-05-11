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

class Wagon
{
public:
	Wagon();
	virtual ~Wagon();
	void init(TextureLoader* texLoader, Terrain* aTerrain, Menu* aMenu, bool* gP, Manager* mgr);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, RenderingHelper* modelTrans);
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
	bool* gamePaused;
	float deltaTime;
	bool wagonStart;
	float velocity;
	glm::vec3 direction;
	glm::vec3 nextPoint;
	glm::vec3 orientation;
};

#endif
