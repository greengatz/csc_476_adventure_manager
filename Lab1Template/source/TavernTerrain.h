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
	static const int MAP_Y = 50;
	static const int MAP_Z = 50;
	static const int MAP_SCALE = 1;
	float terrain[MAP_X][MAP_Z][3]; // Terrain data
	float wallData[MAP_X][MAP_Y][3]; // wall data
	float wallData2[MAP_X][MAP_Y][3];
	GLuint posBufID, norBufID, texBufID;
	GLuint wallPosBufID, wallNorBufID, wallTexBufID;
	GLuint wall2PosBufID, wall2NorBufID, wall2TexBufID;
	glm::vec3 ground; // position
	glm::vec3 wall1, wall2;
	float scale;
	void setUpStack(RenderingHelper *modelTrans, GLint h_ModelMatrix, glm::vec3 target);
	void drawATex(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, GLuint posBuffer, GLuint norBuffer, GLuint texBuffer, int targetTex);
	void calcBuffers(int coor1, int coor2, GLuint *posBuf, GLuint *norBuf, GLuint *texBuf, float data[MAP_X][MAP_Z][3]);
};

#endif
