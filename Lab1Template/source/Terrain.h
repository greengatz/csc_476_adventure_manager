/**
 * Terrain.h - Header file for Terrain object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include "TerrainEvent.h"
#include <vector>
#include "splineCurve.h"
#include "Tree.h"
#include "Camera.h"
#include "Materials.h"

#define MERCHANT 1
#define WANDERER 2
#define AMBUSH 3
#define SICKNESS 4

class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void init(TextureLoader* texLoader, Materials *matSetter, FrustumCull *fCuller);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, Camera* camera, glm::vec3 wagonPos, GLuint* pid);
	void createTrail();
	void createEvents();
	glm::vec3 getStartPosition();
	bool atEnd(glm::vec3 aPos);
	glm::vec3 nextCriticalPoint(glm::vec3 aPos);
	void printCriticalPoints();
	int checkEvents(glm::vec3 aPos);
	void placeEvents();
    Spline* getSpline();

private:

	static const int MAP_X = 50;
	static const int MAP_Z = 50;
	static const int MAP_SCALE = 1;
	float terrain[MAP_X][MAP_Z][3]; // Terrain data
	float randomTree[MAP_X][MAP_Z]; // random tree
	GLuint posBufID;
	GLuint norBufID;
	GLuint texBufID;
	glm::vec3 x; // position
	float scale;
	typedef int TArray[MAP_X][MAP_Z];
	typedef int EArray[MAP_X];
	TArray trailMap;
	EArray eventsMap;
	glm::vec3 beginPosition;
	std::vector<glm::vec3> criticalPoints;
	int oldTextureID;
	TerrainEvent terrainEvents;
    int nextCPoint = 1;
    Spline* path;
    Tree tree;
    int startingSpot;


};

#endif
