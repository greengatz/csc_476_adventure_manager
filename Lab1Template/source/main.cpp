#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/**
 * Mercenary Manager
 * @author Brandon Clark
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "GLSL.h"
#include "Camera.h"
#include "Terrain.h"
#include "hud.h"
#include "menu.h"
#include "MatrixStack.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "RenderingHelper.h"
#include "TextureLoader.h"
#include "obj3d.h"
#include "obj3dcontainer.h"
#include "tavern.h"
#include "Wagon.h"
#include "manager.h"
#include "TavernTerrain.h"
#include "Materials.h"
#include "FrustumCull.h"
#include <string>
#include "splineTest.cpp"
#include "TerrainEvent.h"
#include "text2D.hpp"
#include "SoundPlayer.h"
#include "Skybox.h"
#include "FadeSystem.h"
#include "ProjectMeshes.h"
#include "FireSystem.h"
#include "ShadowMapFBO.h"
#include "ShadowMapTechnique.h"

using namespace std;
using namespace glm;

void idleGL();

float rF(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}

TextureLoader texLoader;

//The Window & window size
GLFWwindow* window;
int g_width;
int g_height;

int points = 0;

Terrain terrain;
//Plane toggle for coloring
GLint terrainToggleID, trailTerrainToggleID, bone_terrainToggleID;
GLint normalToggleID;

Wagon wagon;

int NUMOBJ = 5;
Camera camera;
bool gamePaused = false;
float fastForward = 1.0f;
bool cull = false;
glm::vec2 mouse;
int shapeCount = 1;
//pid used for glUseProgram(pid);
GLuint pid, trailPid, bonePid;
GLint h_vertPos, h_trail_vertPos, h_bone_vertPos;
GLint h_vertNor, h_trail_vertNor, h_bone_vertNor;
GLint h_aTexCoord, h_trail_aTexCoord, h_bone_aTexCoord, h_uNorUnit;
//Handles to the shader data
GLint h_uTexUnit, h_u2DNor, h_trail_uTexUnit, h_bone_uTexUnit;
GLint h_ProjMatrix, h_trail_ProjMatrix, h_bone_ProjMatrix;
GLint h_ViewMatrix, h_trail_ViewMatrix, h_bone_ViewMatrix;
GLint h_ModelMatrix, h_trail_ModelMatrix, h_bone_ModelMatrix;

GLint h_lightPos1, h_trail_lightPos1, h_bone_lightPos1;
GLint h_lightPos2, h_trail_lightPos2, h_bone_lightPos2;
GLint h_ka, h_trail_ka, h_bone_ka;
GLint h_kd, h_trail_kd;
GLint h_ks, h_trail_ks; // we don't need these for bones
GLint h_s, h_trail_s;
GLint h_trail_option, h_bone_option;
GLint h_trail_flag, h_bone_flag;

// bone handles
GLint h_boneFlag;
GLint h_boneIds;
GLint h_boneWeights;
GLint h_boneIds2;
GLint h_boneWeights2;
GLint h_boneTransforms;

bool keyToggles[256] = {false};
float t = 0.0f;
float h = 0.1f;
glm::vec3 location(0.0f,0.0f,0.0f);
glm::vec3 g(-10.0f, -5.0f, 0.0f);

// Display time to control fps
float timeOldDraw = 0.0f;
float timeNew = 0.0f;
float timeOldSpawn = 0.0f;
bool hudHelper = true;
float timeOldHUD = 0.0f;
float timeNewHUD = 0.0f;;
float dtHUD = 0.0f;

typedef struct{
	float x;
	float y;
	float z;
}threeFloat;

typedef struct{
	threeFloat ka;
	threeFloat kd;
	threeFloat ks;
	float s;
}Material;

int matCount = 2;
float optionS = 0.0f;
// int g_GiboLen;
GLuint NumBufObj, NumIndBufObj, NumTexBufObj;

//Rendering Helper
RenderingHelper ModelTrans;
Tavern tavern;
Manager manager("The Dude");
TavernTerrain tavTerr;
Materials matSetter, trailMatSetter;
FrustumCull fCuller;
HUD hud(&manager);
Menu menu;
double dtDraw;
SoundPlayer audio;
ProjectMeshes meshes;
FireSystem fire;

// TerrainEvent terrEv; //this is only here for testing purposes

//The skybox
Skybox skybox;

//Fading in and out
FadeSystem fadeSystem;

//Shadow Information
ShadowMapTechnique* m_pShadowMapTech;
//TODO: Eventually have this light move across with wagon
//or have multiple lights across the trail.
glm::vec3 outsideLightPos = glm::vec3(-90.0, 15.0, -53.0);
glm::vec3 outsideLightLookat = glm::vec3(-90.0, 0.0, -29.0);
ShadowMapFBO m_shadowMapFBO;

vector<Obj3d> objs;

//Spatial Grid
vector<vector<vector<Obj3d>>> grid;
int minX = 5;
int minZ = -40;
int gridSize = 7;

void initModels()
{
	//Initialize meshes
	meshes.loadMeshes();

	//Initialize the fade in/out system
	fadeSystem.init();


	//Initialize Tavern object
	tavern.init(&matSetter, &fCuller, &meshes, &normalToggleID);


	//Initialize Terrain object
	terrain.init(&texLoader, &trailMatSetter, &fCuller, &meshes, outsideLightPos);
	tavTerr.init(&texLoader);

	//Initialize Manager object
	manager.init(&menu, &gamePaused, &fadeSystem, &audio);

	//Initalize Wagon
	wagon.init(&texLoader, &terrain, &menu, &gamePaused, &fastForward, &manager, &meshes, &audio);

	//Initialize skybox
	skybox.init(&texLoader);

	//Initialize the fire particle system
	fire.init(&texLoader);
	

	//Initalize some shadow information
	m_shadowMapFBO.init(g_width, g_height);
	m_pShadowMapTech = new ShadowMapTechnique();
	m_pShadowMapTech->init();

	//initialize the modeltrans matrix stack
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

}

void initGL()
{	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);

	/* texture specific settings */
    glEnable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

bool installBoneShader(const string &vShaderName, const string &fShaderName)
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
	GLSL::printError();
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(VS);
	if(!rc) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
	}
	
	// Compile fragment shader
	glCompileShader(FS);
	GLSL::printError();
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(FS);
	if(!rc) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
	}
	
	// Create the program and link
	bonePid = glCreateProgram();
	glAttachShader(bonePid, VS);
	glAttachShader(bonePid, FS);
	glLinkProgram(bonePid);
	GLSL::printError();
	glGetProgramiv(pid, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(bonePid);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
	}
	
	h_bone_vertPos = GLSL::getAttribLocation(bonePid, "vertPos");
	h_bone_vertNor = GLSL::getAttribLocation(bonePid, "vertNor");
	h_bone_aTexCoord = GLSL::getAttribLocation(bonePid, "aTexCoord");
	h_bone_ProjMatrix = GLSL::getUniformLocation(bonePid, "uProjMatrix");
	h_bone_ViewMatrix = GLSL::getUniformLocation(bonePid, "uViewMatrix");
	h_bone_ModelMatrix = GLSL::getUniformLocation(bonePid, "uModelMatrix");

	h_bone_uTexUnit = GLSL::getUniformLocation(bonePid, "uTexUnit");
	h_bone_ka = GLSL::getUniformLocation(bonePid, "ka");

	h_bone_lightPos1 = GLSL::getUniformLocation(bonePid, "lightPos1");
	h_bone_lightPos2 = GLSL::getUniformLocation(bonePid, "lightPos2");
	h_bone_option = GLSL::getUniformLocation(bonePid, "option");
	h_bone_flag = GLSL::getUniformLocation(bonePid, "flag");

    h_boneFlag = GLSL::getUniformLocation(bonePid, "boneToggle");
    h_boneIds = GLSL::getAttribLocation(bonePid, "boneIds");
    h_boneWeights = GLSL::getAttribLocation(bonePid, "boneWeights");
    h_boneIds2 = GLSL::getAttribLocation(bonePid, "boneIds2");
    h_boneWeights2 = GLSL::getAttribLocation(bonePid, "boneWeights2");
    h_boneTransforms = GLSL::getUniformLocation(bonePid, "bones");

	/*Toggle for plane coloring*/
    bone_terrainToggleID = GLSL::getUniformLocation(bonePid, "terrainToggle");

	// Check GLSL
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

/**
 * Initialize the shaders passed to the function TODO--> There is already a class that does this.....
 */
bool installTavShader(const string &vShaderName, const string &fShaderName)
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
	GLSL::printError();
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(VS);
	if(!rc) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
	}
	
	// Compile fragment shader
	glCompileShader(FS);
	GLSL::printError();
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(FS);
	if(!rc) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
	}
	
	// Create the program and link
	pid = glCreateProgram();
	glAttachShader(pid, VS);
	glAttachShader(pid, FS);
	glLinkProgram(pid);
	GLSL::printError();
	glGetProgramiv(pid, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(pid);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
	}
	
	h_vertPos = GLSL::getAttribLocation(pid, "vertPos");
	h_vertNor = GLSL::getAttribLocation(pid, "vertNor");
	h_aTexCoord = GLSL::getAttribLocation(pid, "aTexCoord");
   h_uNorUnit = GLSL::getAttribLocation(pid, "normCoord");
	h_ProjMatrix = GLSL::getUniformLocation(pid, "uProjMatrix");
	h_ViewMatrix = GLSL::getUniformLocation(pid, "uViewMatrix");
	h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");

	h_uTexUnit = GLSL::getUniformLocation(pid, "uTexUnit");
   h_u2DNor = GLSL::getUniformLocation(pid, "uTexNorUnit");

	h_lightPos1 = GLSL::getUniformLocation(pid, "lightPos1");
	h_ka = GLSL::getUniformLocation(pid, "ka");
	h_kd = GLSL::getUniformLocation(pid, "kd");
	h_ks = GLSL::getUniformLocation(pid, "ks");
	h_s = GLSL::getUniformLocation(pid, "s");

	/*Toggle for plane coloring*/
   terrainToggleID = GLSL::getUniformLocation(pid, "terrainToggle");
   normalToggleID = GLSL::getUniformLocation(pid, "normalToggle");

    matSetter.init(&pid, &h_ka, &h_kd, &h_ks, &h_s);

	// Check GLSL
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

/**
 * Initialize the shaders passed to the function
 */
bool installTrailShader(const string &vShaderName, const string &fShaderName)
{		
	GLint rc2;

	// Create shader handles
	GLuint VS2 = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS2 = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader2 = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader2 = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS2, 1, &vshader2, NULL);
	glShaderSource(FS2, 1, &fshader2, NULL);
	
	// Compile vertex shader
	glCompileShader(VS2);
	GLSL::printError();
	glGetShaderiv(VS2, GL_COMPILE_STATUS, &rc2);
	GLSL::printShaderInfoLog(VS2);
	if(!rc2) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
	}
	
	// Compile fragment shader
	glCompileShader(FS2);
	GLSL::printError();
	glGetShaderiv(FS2, GL_COMPILE_STATUS, &rc2);
	GLSL::printShaderInfoLog(FS2);
	if(!rc2) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
	}
	
	// Create the program and link
	trailPid = glCreateProgram();
	glAttachShader(trailPid, VS2);
	glAttachShader(trailPid, FS2);
	glLinkProgram(trailPid);
	GLSL::printError();
	glGetProgramiv(trailPid, GL_LINK_STATUS, &rc2);
	GLSL::printProgramInfoLog(trailPid);
	if(!rc2) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
	}
	
	h_trail_vertPos = GLSL::getAttribLocation(trailPid, "vertPos");
	h_trail_vertNor = GLSL::getAttribLocation(trailPid, "vertNor");
	h_trail_aTexCoord = GLSL::getAttribLocation(trailPid, "aTexCoord");
	h_trail_ProjMatrix = GLSL::getUniformLocation(trailPid, "uProjMatrix");
	h_trail_ViewMatrix = GLSL::getUniformLocation(trailPid, "uViewMatrix");
	h_trail_ModelMatrix = GLSL::getUniformLocation(trailPid, "uModelMatrix");

	h_trail_uTexUnit = GLSL::getUniformLocation(trailPid, "uTexUnit");

	h_trail_lightPos1 = GLSL::getUniformLocation(trailPid, "lightPos1");
	h_trail_lightPos2 = GLSL::getUniformLocation(trailPid, "lightPos2");
	h_trail_ka = GLSL::getUniformLocation(trailPid, "ka");
	h_trail_kd = GLSL::getUniformLocation(trailPid, "kd");
	h_trail_ks = GLSL::getUniformLocation(trailPid, "ks");
	h_trail_s = GLSL::getUniformLocation(trailPid, "s");
	h_trail_option = GLSL::getUniformLocation(trailPid, "option");
	h_trail_flag = GLSL::getUniformLocation(trailPid, "flag");
	
    /*Toggle for plane coloring*/
    trailTerrainToggleID = GLSL::getUniformLocation(trailPid, "terrainToggle");

    trailMatSetter.init(&trailPid, &h_trail_ka, &h_trail_kd, &h_trail_ks, &h_trail_s);

	// Check GLSL
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

//TODO: RIGHT NOW THIS ONLY CREATES LIGHT MAP FOR WAGON. MUST EXPAND TO EVERYTHING.
/*void ShadowMapPass()
{
	m_pShadowMapTech->enableProgram();

	m_shadowMapFBO.bindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);

	camera.setShadowMapPass(outsideLightPos, outsideLightLookat);

	ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
			ModelTrans.translate(glm::vec3(-100.0, 0.0, 0.0));
			glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
			ModelTrans.pushMatrix();
				wagon.draw(h_trail_vertPos, h_trail_vertNor, h_trail_aTexCoord, h_trail_ModelMatrix, &ModelTrans);
			ModelTrans.popMatrix();
		ModelTrans.popMatrix();

	camera.finishShadowMapPass();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}*/

void setProjView(GLint *projMat, GLint *viewMat)
{
	//Set projection matrix
	MatrixStack proj, view;
	proj.pushMatrix();
	camera.applyProjectionMatrix(&proj);
	glUniformMatrix4fv( (*projMat), 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	proj.pushMatrix();
	camera.applyViewMatrix(&view, wagon.getPosition());
	glUniformMatrix4fv((*viewMat), 1, GL_FALSE, glm::value_ptr(view.topMatrix()));
	
	fCuller.setProjMat(proj.topMatrix(), view.topMatrix());
}

//NOTE: Please do not delete any of the shadow pass comments. :)
void drawGL()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Probably replace this sometime with different shaders....
	if (camera.isTavernView()) {
		glUseProgram(pid);
	}
	else {
		glUseProgram(trailPid);
		glUniform1i(h_trail_flag, 0);
	}


	//Update Camera
	// Get mouse position
  	double xpos, ypos;

  	glfwGetCursorPos(window, &xpos, &ypos);
	camera.update(xpos, ypos, wagon.getPosition());

	if (camera.isTavernView()) {
		glUniform3fv(h_lightPos1, 1, glm::value_ptr(glm::vec3(15.5f, 5.0f, -23.0f)));
      glUniform1i(normalToggleID, 0);
	}
	else {
		glUniform3fv(h_trail_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
		glUniform3fv(h_trail_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
		glUniform1f(h_trail_option, optionS);
	}
	
	// Bind the program
	//Set projection matrix
    mat4 savedProj, savedView;
	MatrixStack proj, view;
	proj.pushMatrix();
	camera.applyProjectionMatrix(&proj);
	if (camera.isTavernView()) {
		glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	}
	else {
		glUniformMatrix4fv( h_trail_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	}
    savedProj = proj.topMatrix();
	proj.pushMatrix();
	camera.applyViewMatrix(&view, wagon.getPosition());
	if (camera.isTavernView()) {
		glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));
	}
	else {
		glUniformMatrix4fv(h_trail_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));
	}
	savedView = view.topMatrix();

	matSetter.setMaterial(2); //is this old code that we should delete?
	trailMatSetter.setMaterial(2);
	
    // send bone transform shader data
 //    glUseProgram(bonePid);
	// glUniformMatrix4fv(h_bone_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));
	// glUniformMatrix4fv( h_bone_ProjMatrix, 1, GL_FALSE, glm::value_ptr(savedMat));
	// glUniform3fv(h_bone_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
	// glUniform3fv(h_bone_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
	// glUniform1f(h_bone_option, optionS);
	
    fCuller.setProjMat(proj.topMatrix(), view.topMatrix());

   // glUseProgram(0);
	
	//matSetter.setMaterial(2); //is this old code that we should delete?
	//trailMatSetter.setMaterial(2);

	//m_shadowMapFBO.bindForWriting();

	//========================== DRAW OUTSIDE SCENE ====================
	if (!camera.isTavernView() || camera.isFreeRoam())
	{
		//Prepass to get light map. 1st render
		//ShadowMapPass();

		//2nd Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(trailPid);
		setProjView(&h_trail_ProjMatrix, &h_trail_ViewMatrix);
		glUniform1i(h_trail_flag, 0);
		glUniform3fv(h_trail_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
		glUniform3fv(h_trail_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
		glUniform1f(h_trail_option, optionS);
      //m_pShadowMapTech->setTextureUnit(0);
		//m_shadowMapFBO.bindForReading(GL_TEXTURE0);

		glUniform1i(trailTerrainToggleID, 1);
		glUniform1i(h_trail_uTexUnit, 0);
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
			ModelTrans.translate(glm::vec3(-100.0, 0.0, 0.0));
			glUniformMatrix4fv(h_trail_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
			ModelTrans.pushMatrix();
				terrain.draw(h_trail_vertPos, h_trail_vertNor, h_trail_aTexCoord, h_trail_ModelMatrix, &camera, wagon.getPosition(), &trailPid);
				wagon.draw(&ModelTrans, &camera, wagon.getPosition());
			ModelTrans.popMatrix();
		ModelTrans.popMatrix();
	            // HERER TODO
               // wagon.drawMercs(h_bone_ModelMatrix, h_bone_vertPos, h_bone_vertNor, 
                 //   h_bone_aTexCoord,  h_boneFlag, h_boneIds, h_boneWeights,
                  //  h_boneTransforms, dtDraw);
		// draw mercs
		// glUseProgram(bonePid);
		// glUniform1i(bone_terrainToggleID, 1);
		// glUniform3f(h_bone_ka, 1.0f, 1.0f, 1.0f);
		// glUniform1i(h_bone_uTexUnit, 0);
		// ModelTrans.loadIdentity();
		// ModelTrans.pushMatrix();
		// ModelTrans.popMatrix();
		// ModelTrans.loadIdentity();
  //       wagon.drawMercs(h_bone_ModelMatrix, h_bone_vertPos, h_bone_vertNor, 
  //                   h_bone_aTexCoord,  h_boneFlag, h_boneIds, h_boneWeights,
  //                   h_boneTransforms, dtDraw, h_boneIds2, h_boneWeights2, bone_terrainToggleID);
		
        glUseProgram(trailPid);
      setProjView(&h_trail_ProjMatrix, &h_trail_ViewMatrix);
		glUniform1i(trailTerrainToggleID, 0);
      //Draw the skybox
      skybox.draw(&camera, wagon.getPosition());
      glUniform1i(trailTerrainToggleID, 0);
	}
	glUseProgram(trailPid);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//m_shadowMapFBO.bindForReading(GL_TEXTURE0);

	//========================= END OUTSIDE SCENE =======================

	if (camera.isTavernView() || camera.isFreeRoam())
	{
		//Draw TAVERN
		glUseProgram(pid);
		glUniform1i(terrainToggleID, 1);
		glUniform1i(h_uTexUnit, 0);
      glUniform1i(h_u2DNor, 1);
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		tavTerr.draw(h_vertPos, h_vertNor, h_aTexCoord, h_uNorUnit, normalToggleID, h_ModelMatrix, &ModelTrans, &matSetter);
		matSetter.setMaterial(4); // TODO does this line do anything?
		ModelTrans.popMatrix();
		matSetter.setMaterial(3);
		tavern.drawTavern(h_ModelMatrix, h_vertPos, h_vertNor, 
                h_aTexCoord, dtDraw, h_boneFlag, h_boneIds, 
                h_boneWeights, h_boneTransforms);
        // load new shader
		// glUseProgram(bonePid);
		// glUniform1i(bone_terrainToggleID, 1);
		// glUniform3f(h_bone_ka, 1.0f, 1.0f, 1.0f);
		// glUniform1i(h_bone_uTexUnit, 0);
		// ModelTrans.loadIdentity();
		// ModelTrans.pushMatrix();
		// ModelTrans.popMatrix();
  //       // push a bunch of data
  //        tavern.drawTavernMercs(h_bone_ModelMatrix, h_bone_vertPos, h_bone_vertNor, 
  //               h_bone_aTexCoord, dtDraw, h_boneFlag, h_boneIds, 
  //               h_boneWeights, h_boneTransforms, bone_terrainToggleID,
  //               h_boneIds2, h_boneWeights2);
		// glUseProgram(0);

		fire.draw(&camera, view.topMatrix()); //draw fire
		glUniform1i(terrainToggleID, 0);

	}

	//===========================Drawing mercenaries============================

	glUseProgram(bonePid);
	  // send bone transform shader data
	glUniform1i(h_bone_flag, 0);
	glUniform3fv(h_bone_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
	glUniform3fv(h_bone_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
	glUniform1f(h_bone_option, optionS);
	
	glUniformMatrix4fv(h_bone_ViewMatrix, 1, GL_FALSE, glm::value_ptr(savedView));
	glUniformMatrix4fv( h_bone_ProjMatrix, 1, GL_FALSE, glm::value_ptr(savedProj));
	glUniform3fv(h_bone_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
	glUniform3fv(h_bone_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
	glUniform1f(h_bone_option, optionS);
	glUniform1i(bone_terrainToggleID, 1);
	glUniform3f(h_bone_ka, 1.0f, 1.0f, 1.0f);
	glUniform1i(h_bone_uTexUnit, 0);

	if (!camera.isTavernView() || camera.isFreeRoam()) {
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		ModelTrans.popMatrix();
		ModelTrans.loadIdentity();
        wagon.drawMercs(h_bone_ModelMatrix, h_bone_vertPos, h_bone_vertNor, 
                    h_bone_aTexCoord,  h_boneFlag, h_boneIds, h_boneWeights,
                    h_boneTransforms, dtDraw, h_boneIds2, h_boneWeights2, bone_terrainToggleID);
	}

	if (camera.isTavernView() || camera.isFreeRoam()) {
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		ModelTrans.popMatrix();
	    // push a bunch of data
	     tavern.drawTavernMercs(h_bone_ModelMatrix, h_bone_vertPos, h_bone_vertNor, 
	            h_bone_aTexCoord, dtDraw, h_boneFlag, h_boneIds, 
	            h_boneWeights, h_boneTransforms, bone_terrainToggleID,
	            h_boneIds2, h_boneWeights2);
 	}
	glUseProgram(0);

	//End of mercenaries

	//****************The fade system******************
	//NOTE: Keep fade system above HUD so we still show hud when fading.

	if (wagon.getPosition().x > -54.0 && !fadeSystem.isFading())
	{
		manager.completedTrail();
		tavern.tavernCharacters.clear();
		tavern.spawn3NewMercs();
		manager.inTavern = manager.inTavern ? false : true;
        audio.playBackgroundMusic(manager.inTavern);
		fadeSystem.startFade(g_width, g_height);
	}

	if (fadeSystem.isFading())
	{
		fadeSystem.updateFade();
		if (fadeSystem.readyToChangeScene())
		{
			if(!fadeSystem.dontToggleView){
				camera.toggleGameViews();
			}else{
				gamePaused = false;
				terrain.createTrail();
   				wagon.resetWagon();
   				manager.blacklisted = false;
   				manager.fortune = false;
				fadeSystem.dontToggleView = false;
			}
			

			printf("isTavernView%d, dontToggleView%d\n",!camera.isTavernView(),!fadeSystem.dontToggleView);
			if (!camera.isTavernView()){
				wagon.startWagon();
			}else{
   				manager.blacklisted = false;
   				manager.fortune = false;
				terrain.createTrail();
   				wagon.resetWagon();
			}
		}
	}
	
	//**************Draw HUD START*********************


	hud.drawHud(&camera, g_width, g_height);
	
	
	if(menu.inMenu)
	{
		menu.drawMenu();
	}
	
	if(hud.on && !camera.isShadowMapView() && !camera.isFreeRoam())
	{
		hud.drawSideHud(&camera, g_width, g_height);

	}
	if(hud.homeScreenOn)
	{
		printText2D("Press Enter to Continue", 75, 75, 24);
	}
	else
	{
		char info[64];
		sprintf(info,"x %d", manager.getGold());
		printText2D(info, 50, 566, 18);

		sprintf(info,"x %d", manager.getFood());
		printText2D(info, 226, 566, 18);

		sprintf(info,"x %d", manager.getBeer());
		printText2D(info, 432, 566, 18);

		sprintf(info,"x %d", manager.getMercs());
		printText2D(info, 628, 566, 18);
	}

	//**************Draw HUD FINISH********************

	// Unbind the program
	glUseProgram(0);
	// Pop stacks
	proj.popMatrix();
}

bool hasCollided(glm::vec3 incr)
{
	if (camera.isFreeRoam())
	{
		return false;
	}

	glm::vec3 camPos = camera.getPosition() + incr;
	// printf("CamPosition: <%lf, %lf, %lf>\n", camPos.x, camPos.y, camPos.z);

	float curCam[6] = {
    camera.bound.minX + camPos.x,
    camera.bound.maxX + camPos.x,
    camera.bound.minY + 1,
    camera.bound.maxY + 1,
    camera.bound.minZ + camPos.z,
    camera.bound.maxZ + camPos.z};

	// bool validMove = (curCam[0] < 6.75 || curCam[1] > 39.5 || curCam[4] < -36.0 || curCam[5] > -11.4);
	bool validMove = (curCam[0] < 6.75 || curCam[1] > 21.5 || curCam[4] < -29.0 || curCam[5] > -16.4);
	//printf("CAMERA AT X: %lf, Z: %lf\n", camPos.x, camPos.z);
	int row, col;
  	row = (camPos.x - minX)/gridSize;
 	col = (camPos.z - minZ)/gridSize;
	vector<Obj3d> currentCellObjs = grid[row][col];

	for(vector<Obj3d>::iterator it1 = currentCellObjs.begin(); it1 != currentCellObjs.end(); ++it1)
	{
		vec3 pos1 = it1 ->getCurSpot();
		if(it1->bound.checkCollision(curCam, it1->scale, pos1))
		{
			validMove = true;
		}
	}

	return validMove;
}

/**
 * This will get called when any button on keyboard is pressed.
 */
void checkUserInput()
{
   float attemptX = numeric_limits<int>::min();
   float attemptZ = numeric_limits<int>::min();
   vec3 view = camera.getLookAtPoint() - camera.getTheEye();
   vec3 strafe = glm::cross(view, vec3(0.0, 1.0, 0.0));
   if (!camera.isTavernView()) {
      return;
   }
   if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS && !hasCollided(-strafe))
   {
      camera.updateStrafe(-strafe);
   }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !hasCollided(strafe))
   {
      camera.updateStrafe(strafe);
   }
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !hasCollided(view*1.2f))
   {
      camera.updateZoom(view*1.2f);
   }
   else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !hasCollided(view))
   {
      camera.updateZoom(view);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !hasCollided(-view*1.2f))
   {
      camera.updateZoom(-view*1.2f);
   }
   else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !hasCollided(-view))
   {
      camera.updateZoom(-view);
   }
}

void mouseScrollCB(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!camera.isTavernView())
	{
		camera.updateWagonZoom(yoffset);
	}
}

/**
 * Use this for debugging purposes for right now.
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	timeNew = glfwGetTime();
	double dtKey = timeNew - timeOldDraw;

	//This time step is causing issues for key inputs right now.
	// Update every 60Hz
	//if(dtKey >= (1.0 / 60.0) ) {
	//Free roam camera
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
   	{
   		camera.toggleFreeRoam();
   	}

   	//Create a new trail
   	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
   	{
   		// terrain.createTrail();
   		// wagon.resetWagon();
   	}

	//Print Manager status
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		// manager.reportStats();
	}
	

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		if(fastForward >= 2.9f)
			fastForward = 1.0f;
		else
			fastForward = 3.0f;

		printf("%f\n", fastForward);
	}


	//Buy food
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		
		if(camera.isTavernView()) {
			manager.buyFood(4);
			audio.playSoundEffect(MEAT_SOUND);
		}
		else{
			if(manager.getFood() > 0){
				manager.feedMerc();
				audio.playSoundEffect(MEAT_SOUND);
			}
		}

	}

	//Buy beer
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		if(camera.isTavernView()) {
			manager.buyBeer(4);
			audio.playSoundEffect(BEER_SOUND);
		}
		else{
			if(manager.getBeer() > 0){
				manager.beerMerc();
				audio.playSoundEffect(BEER_SOUND);
			}
		}
	}
	
	if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5 && action == GLFW_PRESS)
	{
		// tavern.buyMercenary(key - GLFW_KEY_1, &manager);
		if(menu.inMenu)
		{
			menu.selectOption(key - GLFW_KEY_1);
		}
		else if(camera.isTavernView())
		{
			hud.on = true;
			manager.buyMercenary(key - GLFW_KEY_1, &tavern);
			// audio.playSoundEffect(COIN_SOUND);
		}
	}
	
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		//manager.buyMercenary(key - GLFW_KEY_1, &tavern);
        //tavern.tavernCharacters[0].wave();
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		//manager.buyMercenary(key - GLFW_KEY_1, &tavern);
        hud.homeScreenOn = false;
	}

	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	{
		//manager.buyMercenary(key - GLFW_KEY_1, &tavern);
        // hud.deadScreenOn = !hud.deadScreenOn;
	}


	if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        // tavern.showMercsForSale();
    }

	//Leave Tavern
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if(manager.mercs.size() >= 1){
	      	manager.inTavern = manager.inTavern ? false : true;
			//camera.toggleGameViews();
			audio.playBackgroundMusic(manager.inTavern);
			fadeSystem.startFade(g_width, g_height);
		}
	}

   	//Toggle between lines and filled polygons
   	if (key == GLFW_KEY_L && action == GLFW_PRESS)
   	{
   //    	GLint polyType;
   //    	glGetIntegerv(GL_POLYGON_MODE, &polyType);
   //    	if(polyType == GL_FILL) {
			// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// } 
			// else 
			// {
			// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			// }
   	}

   	//Toggle culling.
   	if (key == GLFW_KEY_K && action == GLFW_PRESS)
   	{
   //    	cull = !cull;
   //    	if(cull) {
			// 	glEnable(GL_CULL_FACE);
			// } 
			// else 
			// {
			// 	glDisable(GL_CULL_FACE);
			// }
   	}
   	//Start wagon
   	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		{
			// wagon.startWagon();
		}
	//}
		//testing frustum culling
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		// fCuller.toggleMode();
		//will be using this to toggle it on and off at a specified points, maybe others too....
	}
	//freezes current projection matrix in for frustum culling
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		// fCuller.holdView();
	}
	//Toggle hud
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		hud.on = !hud.on;
	}

	//if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    //{
    	//Create about vector and add an element
	 //  	vector<string> about;
		// about.push_back("about test");

		// //Create an option and add it to a vector
		// option testOpt = {"test option", test};
		// vector<option> options;
		// options.push_back(testOpt);

		// //Set the data
		// menu.setData("Title", about, options);
  //       gamePaused = !gamePaused;
  //       if(!gamePaused){
  //       	printf("%s\n", "reseting start time");
  //       	wagon.setTimeStamp(glfwGetTime());
  //       }
        
    //}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		// fire.toggle();
		// tavern.newEmblem();
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		audio.pause();
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		// audio.playSoundEffect(EXPLOSION_SOUND);
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		// audio.playVoice(BANDIT_GREETING);
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		hud.changeSideHudPage(1);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		hud.changeSideHudPage(-1);
	}

	//DO NOT DELETE THE BELOW CODE THAT IS COMMENTED!!!!!!!!!!, OKAY I PROMISE I WONT -> LOL

	/*if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (camera.isShadowMapView())
		{
			camera.finishShadowMapPass();
		}
		else
		{
			camera.setShadowMapPass(glm::vec3(-91.0, 15.0, -53.0), glm::vec3(-91.0, 0.0, -29.0));
			if (camera.isTavernView())
			{
				camera.toggleGameViews();
			}
			else if (camera.isFreeRoam())
			{
				camera.toggleFreeRoam();
			}
		}
	}*/
}

void window_size_callback(GLFWwindow* window, int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	g_width = w;
	g_height = h;
	camera.setWindowSize(w, h);
}

/**
 * Models that use animation should use this udpate function.
 **/
void updateModels()
{
	wagon.updateWagon(dtDraw);
}

int main(int argc, char **argv)
{
    // please don't remove this, using it to demo splines
    // splineTest(); 

	// Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    //Initialize Spatial Grid
    grid.resize(5);
  	for (int i = 0; i < 5; ++i) {
    	grid[i].resize(5);
  	}

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
   g_width = 1024;
   g_height = 768;
   window = glfwCreateWindow( g_width, g_height, "Mercenary Manager", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, mouseScrollCB);

    // Initialize glad
   if (!gladLoadGL()) {
      fprintf(stderr, "Unable to initialize glad");
      glfwDestroyWindow(window);
      glfwTerminate();
      return 1;
   }

  	// Ensure we can capture the escape key being pressed below
  	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  	glfwSetCursorPos(window, 0.0, 0.0);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initGL();
	installTavShader("Shaders/tav_vert.glsl", "Shaders/tav_frag.glsl");
	installTrailShader("Shaders/trail_vert.glsl", "Shaders/trail_frag.glsl");
	installBoneShader("Shaders/bone_vert.glsl", "Shaders/tav_frag.glsl");
	fCuller.init();
	// terrEv.init(&matSetter, &fCuller, &meshes);

	menu.initMenu(&camera, &texLoader, g_width, g_height, &manager, &gamePaused);
  	
  	initModels();
  	tavern.loadTavernMeshes(&texLoader);

 	//used only for testing purposes
  	// vec3 loc = terrain.getStartPosition();
  	// terrEv.addMerchantStand(vec3(loc.x - 95.5, loc.y, loc.z), glm::mat4(1.0f));
  	// terrEv.addMerchantStand(vec3(loc.x - 92.5, loc.y, loc.z), glm::rotate(glm::mat4(1.0f), (const float)90, glm::vec3(0, 1.0f, 0)));
  	// terrEv.addMerchantStand(vec3(loc.x - 89.5, loc.y, loc.z), glm::rotate(glm::mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)));
  	// terrEv.addEndCity(vec3(loc.x - 82.5, loc.y, loc.z));

   //HUD information
  	hud.initHUD(&texLoader);
  	hud.initSideHud(&texLoader);
  	hud.initHomeScreen(&texLoader);
  	initText2D( "Holstein.DDS" );
  	// initText2D( "uvmap.DDS" );
  	// initText2D( "aText.bmp" );
  	// initText2D("aText.dds");
  	dtDraw = 0;
  	audio.playBackgroundMusic(true);

 	//vector<string> about;
	// about.push_back("about test");

	// //Create an option and add it to a vector
	// option testOpt = {"test option", test};
	// vector<option> options;
	// options.push_back(testOpt);

	// //Set the data
	// menu.setData("Title", about, options);

	objs = tavern.tavernItems;
	int exptX, exptZ, loopi, loopj, col, row;

	for(int k = 6; k < objs.size(); k++)
    {
    	vec3 curPos = objs[k].getCurSpot();
    	//printf("curPos x: %lf, curPos z: %lf\n", curPos.x, curPos.z);
    	float curBox[6] = {
     	objs[k].bound.minX * objs[k].scale.x + curPos.x, 
     	objs[k].bound.maxX * objs[k].scale.x + curPos.x,
     	objs[k].bound.minY * objs[k].scale.y,
     	objs[k].bound.maxY * objs[k].scale.y,
     	objs[k].bound.minZ * objs[k].scale.z + curPos.z,
     	objs[k].bound.maxZ * objs[k].scale.z + curPos.z};

    // exptX = (objs[k].bound.maxX - objs[k].bound.minX);
    // loopi = std::max((int)exptX, 1);
    // exptZ = (objs[k].bound.maxZ - objs[k].bound.minZ);
    // loopj = std::max((int)exptZ, 1);

    	//printf("maxX: %lf, minX: %lf\n", curBox[1], curBox[0]);

    	exptX = (curBox[1] - curBox[0])/gridSize;
    	loopi = std::max((int)exptX + 1, 1);
    	exptZ = (curBox[5] - curBox[4])/gridSize;
    	loopj = std::max((int)exptZ + 1, 1);

    	//printf("exptX: %d, exptZ: %d\n", exptX, exptZ);

    	col = (curBox[0] - minX)/gridSize;
   		row = (curBox[4] - minZ)/gridSize;

    	//printf("col: %d, row: %d\n", col, row);

    	// printf("loopi = %d, loopj = %d\n", loopi, loopj);

    	for(int i = col; i < loopi + col; i++)
    	{
      		for(int j = row; j < loopj + row; j++)
      		{
       		  grid[i][j].push_back(objs[k]);
      		  //printf("gridsize at %d, %d: %d\n", i, j, grid[i][j].size());
     		 }
    	}
  	}

   do{
   		timeNew = glfwGetTime();
		audio.checkTime();
		dtDraw = timeNew - timeOldDraw;
		t += h;
		// Update every 60Hz
		if(dtDraw >= (1.0 / 60.0) ) {
			checkUserInput();
			updateModels();
			timeOldDraw += (1.0 / 60.0);
			//Draw an image
			drawGL();
			if(camera.isTavernView()){
				if(hud.on){
					if(hudHelper)
						timeNewHUD = timeOldHUD = glfwGetTime();
					else
						timeNewHUD = glfwGetTime();
					hudHelper = false;
					dtHUD = timeNewHUD - timeOldHUD;
					if(dtHUD >= 4.0){
						hud.on = false;
						hudHelper = true;
					}
				}else{
					hudHelper = true;
					timeNewHUD = timeOldDraw;
				}
			}
		}
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

   } // Check if the ESC key was pressed or the window was closed
   while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

   // Close OpenGL window and terminate GLFW
   glfwTerminate();

	return 0;
}
