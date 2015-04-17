/**
 * Terrain.h - Header file for Terrain object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _WALL_H_
#define _WALL_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
 #include "TextureLoader.h"

class Wall
{
public:
	Wall();
	virtual ~Wall();
	void init(TextureLoader* texLoader);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord);
	
private:
	std::vector<tinyobj::shape_t> shapes;
	GLuint posBufID;
	GLuint norBufID;
	GLuint texBufID;
	GLuint indBufID;
	glm::vec3 x; // position
	float scale;
};

#endif
