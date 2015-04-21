//
// sueda
// October, 2014
//

#include <iostream>
#include <cassert>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Program.h"
#include "GLSL.h"
#include "Texture.h"

using namespace std;

Program::Program() :
	pid(0),
	vShaderName(""),
	fShaderName("")
{
	
}

Program::~Program()
{
	
}

void Program::setShaderNames(const string &v, const string &f)
{
	vShaderName = v;
	fShaderName = f;
}

bool Program::init()
{
	GLint rc;
	
	// Create shader handles
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS, 1, &vshader, NULL);
	glShaderSource(FS, 1, &fshader, NULL);
	
	// Compile vertex shader
	glCompileShader(VS);
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(VS);
	if(!rc) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
		return false;
	}
	
	// Compile fragment shader
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(FS);
	if(!rc) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
		return false;
	}
	
	// Create the program and link
	pid = glCreateProgram();
	glAttachShader(pid, VS);
	glAttachShader(pid, FS);
	glLinkProgram(pid);
	glGetProgramiv(pid, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(pid);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
		return false;
	}
	
	addAttribute("vertPosition");
	addAttribute("vertTexCoords");
	addUniform("P");
	addUniform("MV");
	addUniform("scale");
	addUniform("color");
	
	GLSL::printError();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

void Program::bind()
{
	glUseProgram(pid);
	for(map<string,Texture*>::iterator it = textures.begin(); it != textures.end(); ++it) {
		it->second->bind();
	}
}

void Program::unbind()
{
	for(map<string,Texture*>::iterator it = textures.begin(); it != textures.end(); ++it) {
		it->second->unbind();
	}
	glUseProgram(0);
}

void Program::addAttribute(const string &name)
{
	attributes[name] = GLSL::getAttribLocation(pid, name.c_str());
}

void Program::addUniform(const string &name)
{
	uniforms[name] = GLSL::getUniformLocation(pid, name.c_str());
}

void Program::addTexture(Texture *texture)
{
	const string &name = texture->getName();
	GLint handle = GLSL::getUniformLocation(pid, name.c_str());
	texture->setHandle(handle);
	textures[name] = texture;
}

GLint Program::getAttribute(const string &name) const
{
	map<string,GLint>::const_iterator attribute = attributes.find(name.c_str());
	if(attribute == attributes.end()) {
		cout << name << " is not an attribute variable" << endl;
		return 0;
	}
	return attribute->second;
}

GLint Program::getUniform(const string &name) const
{
	map<string,GLint>::const_iterator uniform = uniforms.find(name.c_str());
	if(uniform == uniforms.end()) {
		cout << name << " is not a uniform variable" << endl;
		return 0;
	}
	return uniform->second;
}

Texture *Program::getTexture(const string &name) const
{
	map<string,Texture*>::const_iterator texture = textures.find(name.c_str());
	if(texture == textures.end()) {
		cout << name << " is not a texture" << endl;
		return 0;
	}
	return texture->second;
}
