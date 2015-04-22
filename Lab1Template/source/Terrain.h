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
#include <vector>


class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void init(TextureLoader* texLoader);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord);
	void createTrail();
	void createEvents();
	glm::vec3 getStartPosition();
	bool atEnd(glm::vec3 aPos);
	glm::vec3 nextCriticalPoint(glm::vec3 aPos);
	void printCriticalPoints();
	void checkEvents(glm::vec3 aPos);

private:

	static const int MAP_X = 50;
	static const int MAP_Z = 50;
	static const int MAP_SCALE = 1;
	float terrain[MAP_X][MAP_Z][3]; // Terrain data
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


};

#endif
