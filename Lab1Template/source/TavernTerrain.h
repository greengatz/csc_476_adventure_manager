/**
 * Terrain.h - Header file for Terrain object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _TAVERNTERRAIN_H_
#define _TAVERNTERRAIN_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include "RenderingHelper.h"
#include <vector>


class TavernTerrain
{
public:
	TavernTerrain();
	virtual ~TavernTerrain();
	void init(TextureLoader* texLoader);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, RenderingHelper *modelTrans);
	
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


};

#endif
