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

class Wagon
{
public:
	Wagon();
	virtual ~Wagon();
	void init(TextureLoader* texLoader);
	void draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord);
	void setPosition(float x, float y, float z);
	void setScale(float aScale);
	void setRotation(float aRotation);
	void startWagon();
	
private:
	std::vector<tinyobj::shape_t> shapes;
	GLuint posBufID;
	GLuint norBufID;
	GLuint texBufID;
	GLuint indBufID;
	glm::vec3 position;
	float scale;
	float rotate;
	float startTime;
};

#endif
