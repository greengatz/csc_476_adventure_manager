//
// sueda
// November, 2014
//

#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif
#include <vector>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

class MatrixStack;
class Program;
class Texture;

class Particle
{
public:
	Particle();
	virtual ~Particle();
	void load();
	void init(Program *prog);
	void draw(MatrixStack *MV) const;
	void rebirth(float t);
	void update(float t, float h, const glm::vec3 &g, const bool *keyToggles);
	const glm::vec3 &getPosition() const { return x; };
	const glm::vec3 &getVelocity() const { return v; };
	
private:
	float charge; // +1 or -1
	float m; // mass
	float d; // viscous damping
	glm::vec3 x; // position
	glm::vec3 v; // velocity
	float lifespan; // how long this particle lives
	float tEnd;     // time this particle dies
	float scale;
	glm::vec4 color;
	std::vector<float> posBuf;
	std::vector<float> texBuf;
	std::vector<unsigned int> indBuf;
	GLuint posBufID;
	GLuint texBufID;
	GLuint indBufID;
	Program *prog;
};

#endif
