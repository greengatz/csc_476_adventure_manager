/**
 * Terrain.h - Header file for Terrain object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include <vector>
#include "Camera.h"
#include "RenderingHelper.h"

class Skybox
{
public:
	Skybox();
	virtual ~Skybox();
	void init(TextureLoader* texLoader);
	void draw(Camera* camera, glm::vec3 wagonPos);

private:
	static const int PLANE_SCALE = 35;
	GLuint pid;
	GLint h_vertPos;
	GLint h_vertNor;
	GLint h_aTexCoord;
	//Handles to the shader data
	GLint h_uTexUnit;
	GLint h_ProjMatrix;
	GLint h_ViewMatrix;
	GLint h_ModelMatrix;

	//Skybox Buffer
	GLuint posBufObjSkybox;
	GLuint norBufObjSkybox;
	GLuint skyTexBuffObj;

	glm::vec3 position;
	//Rendering Helper
	RenderingHelper ModelTrans;
};

#endif