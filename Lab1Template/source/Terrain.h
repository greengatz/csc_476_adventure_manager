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


class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void init(TextureLoader* texLoader);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord);
	void createTrail();
	
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
	TArray trailMap;


};

#endif
