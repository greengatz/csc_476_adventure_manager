//
// sueda
// November, 2014
//

#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "Particle.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"

using namespace std;

float randFloat(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}

Particle::Particle() :
	charge(1.0f),
	m(1.0f),
	d(0.0f),
	x(0.0f, 0.0f, 0.0f),
	v(0.0f, 0.0f, 0.0f),
	lifespan(1.0f),
	tEnd(0.0f),
	scale(1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	posBufID(0),
	texBufID(0),
	indBufID(0)
{
}

Particle::~Particle()
{
}

void Particle::load()
{
	// Load geometry
	// 0
	posBuf.push_back(-0.5f);
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	// 1
	posBuf.push_back(0.5f);
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(0.0f);
	// 2
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	// 3
	posBuf.push_back(0.5f);
	posBuf.push_back(0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(1.0f);
	// indices
	indBuf.push_back(0);
	indBuf.push_back(1);
	indBuf.push_back(2);
	indBuf.push_back(3);
	
	// Random initialization
	rebirth(0.0f);
}

void Particle::init(Program *prog)
{
	this->prog = prog;
	
	// Send the position array to the GPU
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the texture coordinates array to the GPU
	glGenBuffers(1, &texBufID);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	glGenBuffers(1, &indBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	assert(glGetError() == GL_NO_ERROR);
}

void Particle::draw(MatrixStack *MV) const
{
	// Don't draw if at the origin
	if(glm::dot(x, x) < 0.01f) {
		return;
	}
	
	// Enable and bind position array for drawing
	GLint h_pos = prog->getAttribute("vertPosition");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind texcoord array for drawing
	GLint h_tex = prog->getAttribute("vertTexCoords");
	GLSL::enableVertexAttribArray(h_tex);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	
	// Transformation matrix
	MV->pushMatrix();
	MV->translate(x);
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	MV->popMatrix();
	
	// Color and scale
	glUniform4fv(prog->getUniform("color"), 1, glm::value_ptr(color));
	glUniform1f(prog->getUniform("scale"), scale);
	
	// Draw
	glDrawElements(GL_TRIANGLE_STRIP, (int)indBuf.size(), GL_UNSIGNED_INT, 0);
	
	// Disable and unbind
	GLSL::disableVertexAttribArray(h_tex);
	GLSL::disableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Particle::rebirth(float t)
{	
	if(randFloat(0.0f, 1.0f) > 0.5f)
		charge = 1.0f;
	else
		charge = -1.0f;
	m = 1.0f;
	d = randFloat(0.0f, 0.00001f);
	x.x = randFloat(-1.0f, 1.0f);
	x.y = randFloat(-1.0f, 1.0f);
	x.z = randFloat(-1.0f, 1.0f);
	v.x = randFloat(-0.1f, 0.1f);
	v.y = randFloat(-0.1f, 0.1f);
	v.z = randFloat(-0.1f, 0.1f);
	lifespan = randFloat(25.0f, 75.0f);
	tEnd = t + lifespan;
	scale = randFloat(0.5f, 1.5f);
	color.x = randFloat(0.0f, 1.0f);
	color.y = randFloat(0.0f, 1.0f);
	color.z = randFloat(0.0f, 1.0f);
	color.w = randFloat(0.0f, 1.0f);
}

void Particle::update(float t, float h, const glm::vec3 &g, const bool *keyToggles)
{
	int C = -1;
	glm::vec3 f = m * g;
	f = f + (-d * v);
	if(keyToggles['g']){
		float r = sqrt(x.x * x.x + x.y * x.y + x.z * x.z);
		if(r > 0.001f)
			f = ((C * m) / r * r) * (x / r) * charge;
	}
	if(t > tEnd) {
		rebirth(t);
	}
	v += (h / m) * f;
	x += h*v;
	color.w = (tEnd-t)/lifespan;
	if(keyToggles['f']){
		if(x.y <= -3.0f){
			v.x = 0.0f - v.x;
			v.y = 0.0f - v.y;
			v.z = 0.0f - v.z;
		}
	}

}
