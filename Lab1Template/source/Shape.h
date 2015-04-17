//
// sueda
// October, 2014
//
#pragma once
#ifndef _SHAPE_H_
#define _SHAPE_H_
#define xBoundMax 48.5
#define xBoundMin 0.5
#define zBoundMax -0.5
#define zBoundMin -48.5
#include "GLSL.h"
#include "tiny_obj_loader.h"



class Shape
{
public:
	Shape();
	virtual ~Shape();
	void load(const std::string &meshName);
	void init(float xR, float zR);
	void draw(GLint h_pos, GLint h_nor);
	void update(float t, float h, const glm::vec3 &g, const bool *keyToggles);
	glm::vec3 getPosition();
	void setColorGreen();
	void freezeShape();
	float getRadius();
	bool isGreen();
	
private:
	std::vector<tinyobj::shape_t> shapes;
	GLuint posBufID;
	GLuint norBufID;
	GLuint indBufID;
	glm::vec3 x; // position
	glm::vec3 v; // velocity
	float scale;
	bool color;
};

#endif
