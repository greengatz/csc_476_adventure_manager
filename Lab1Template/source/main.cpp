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
GLint terrainToggleID;

Wagon wagon;

int NUMOBJ = 5;
Camera camera;
bool gamePaused = false;
bool cull = false;
glm::vec2 mouse;
int shapeCount = 1;
//pid used for glUseProgram(pid);
GLuint pid;
GLint h_vertPos;
GLint h_vertNor;
GLint h_aTexCoord;
//Handles to the shader data
GLint h_uTexUnit;
GLint h_ProjMatrix;
GLint h_ViewMatrix;
GLint h_ModelMatrix;

GLint h_lightPos1;
GLint h_lightPos2;
GLint h_ka;
GLint h_kd;
GLint h_ks;
GLint h_s;
GLint h_option;
GLint h_flag;
GLint h_inTav;

bool keyToggles[256] = {false};
float t = 0.0f;
float h = 0.1f;
glm::vec3 location(0.0f,0.0f,0.0f);
glm::vec3 g(-10.0f, -5.0f, 0.0f);

// Display time to control fps
float timeOldDraw = 0.0f;
float timeNew = 0.0f;
float timeOldSpawn = 0.0f;

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
Materials matSetter;
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

/**
 * For now, this just initializes the Shape object.
 * Later, we'll updated to initialize all objects moving.
 * (This is very specific right now to one object).....
 */
void initShape(char * filename)
{
	t = 0.0f;
	h = 0.001f;

	//Initialize shapes here
}

void initModels()
{
	//Initialize meshes
	meshes.loadMeshes();

	//Initialize Tavern object
	tavern.init(&matSetter, &fCuller, &meshes);

	//Initialize Manager object
	manager.init(&menu, &gamePaused);

	//Initialize Terrain object
	terrain.init(&texLoader, &matSetter, &fCuller, &meshes, outsideLightPos);
	tavTerr.init(&texLoader);

	//Initalize Wagon
	wagon.init(&texLoader, &terrain, &menu, &gamePaused, &manager, &meshes, &audio);

	//Initialize skybox
	skybox.init(&texLoader);

	//Initialize the fade in/out system
	fadeSystem.init();

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

/**
 * Initialize the shaders passed to the function
 */
bool installShaders(const string &vShaderName, const string &fShaderName)
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
	h_ProjMatrix = GLSL::getUniformLocation(pid, "uProjMatrix");
	h_ViewMatrix = GLSL::getUniformLocation(pid, "uViewMatrix");
	h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");

	h_uTexUnit = GLSL::getUniformLocation(pid, "uTexUnit");

	h_lightPos1 = GLSL::getUniformLocation(pid, "lightPos1");
	h_lightPos2 = GLSL::getUniformLocation(pid, "lightPos2");
	h_ka = GLSL::getUniformLocation(pid, "ka");
	h_kd = GLSL::getUniformLocation(pid, "kd");
	h_ks = GLSL::getUniformLocation(pid, "ks");
	h_s = GLSL::getUniformLocation(pid, "s");
	h_option = GLSL::getUniformLocation(pid, "option");
	h_flag = GLSL::getUniformLocation(pid, "flag");
	h_inTav = GLSL::getUniformLocation(pid, "inTav");

	/*Toggle for plane coloring*/
    terrainToggleID = GLSL::getUniformLocation(pid, "terrainToggle");

    matSetter.init(&pid, &h_ka, &h_kd, &h_ks, &h_s);

	// Check GLSL
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

//TODO: RIGHT NOW THIS ONLY CREATES LIGHT MAP FOR WAGON. MUST EXPAND TO EVERYTHING.
void ShadowMapPass()
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
				wagon.draw(h_vertPos, h_vertNor, h_aTexCoord, h_ModelMatrix, &ModelTrans);
			ModelTrans.popMatrix();
		ModelTrans.popMatrix();

	camera.finishShadowMapPass();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void setProjView()
{
	//Set projection matrix
	MatrixStack proj, view;
	proj.pushMatrix();
	camera.applyProjectionMatrix(&proj);
	glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	proj.pushMatrix();
	camera.applyViewMatrix(&view, wagon.getPosition());
	glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));
	
	fCuller.setProjMat(proj.topMatrix(), view.topMatrix());
}

void drawGL()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Probably replace this sometime with different shaders....
	glUseProgram(pid);
	glUniform1i(h_flag, 0);

	//Update Camera
	// Get mouse position
  	double xpos, ypos;

  	glfwGetCursorPos(window, &xpos, &ypos);
	camera.update(xpos, ypos, wagon.getPosition());
    //printf("camera at %f, %f\n", camera.getPosition().x, camera.getPosition().z); // TODO remove this

	glUniform3fv(h_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
	glUniform3fv(h_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
	glUniform1f(h_option, optionS);
	
	// Bind the program
	//Set projection matrix
	MatrixStack proj, view;
	proj.pushMatrix();
	camera.applyProjectionMatrix(&proj);
	glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	proj.pushMatrix();
	camera.applyViewMatrix(&view, wagon.getPosition());
	glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));
	
	fCuller.setProjMat(proj.topMatrix(), view.topMatrix());
	
	matSetter.setMaterial(2);

	m_shadowMapFBO.bindForWriting();

	//========================== DRAW OUTSIDE SCENE ====================
	if (!camera.isTavernView() || camera.isFreeRoam())
	{
		//Prepass to get light map. 1st render
		ShadowMapPass();

		//2nd Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(pid);
		setProjView();
		glUniform1i(h_flag, 0);
		glUniform1i(h_inTav, 0);
		glUniform3fv(h_lightPos1, 1, glm::value_ptr(glm::vec3(23.05f, 4.0f, -23.5f)));
		glUniform3fv(h_lightPos2, 1, glm::value_ptr(glm::vec3(-125.0f, 4.0f, 25.0f)));
		glUniform1f(h_option, optionS);
      m_pShadowMapTech->setTextureUnit(0);
		m_shadowMapFBO.bindForReading(GL_TEXTURE0);

		glUniform1i(terrainToggleID, 1);
		glUniform1i(h_uTexUnit, 0);
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
			ModelTrans.translate(glm::vec3(-100.0, 0.0, 0.0));
			glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
			ModelTrans.pushMatrix();
				terrain.draw(h_vertPos, h_vertNor, h_aTexCoord, h_ModelMatrix, &camera, wagon.getPosition(), &pid);
            glUseProgram(pid);
            setProjView();
				wagon.draw(h_vertPos, h_vertNor, h_aTexCoord, h_ModelMatrix, &ModelTrans);
			ModelTrans.popMatrix();
		ModelTrans.popMatrix();
		
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		ModelTrans.popMatrix();
		glUniform1i(terrainToggleID, 0);
      //Draw the skybox
      skybox.draw(&camera, wagon.getPosition());

      glUniform1i(terrainToggleID, 0);
	}
	glUseProgram(pid);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_shadowMapFBO.bindForReading(GL_TEXTURE0);

	//========================= END OUTSIDE SCENE =======================

	if (camera.isTavernView() || camera.isFreeRoam())
	{
		//Draw TAVERN
		glUniform1i(terrainToggleID, 1);
		glUniform1i(h_inTav, 1);
		glUniform1i(h_uTexUnit, 0);
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		tavTerr.draw(h_vertPos, h_vertNor, h_aTexCoord, h_ModelMatrix, &ModelTrans);
		matSetter.setMaterial(4); // TODO does this line do anything?
		ModelTrans.popMatrix();
		matSetter.setMaterial(3);
		tavern.drawTavern(h_ModelMatrix, h_vertPos, h_vertNor, h_aTexCoord, dtDraw);
		fire.draw(&camera, view.topMatrix()); //draw fire
		glUniform1i(terrainToggleID, 0);
	}

	//****************The fade system******************
	//NOTE: Keep fade system above HUD so we still show hud when fading.

	if (wagon.getPosition().x > -54.0 && !fadeSystem.isFading())
	{
		manager.inTavern = manager.inTavern ? false : true;
        audio.playBackgroundMusic(manager.inTavern);
		fadeSystem.startFade(g_width, g_height);
	}

	if (fadeSystem.isFading())
	{
		fadeSystem.updateFade();
		if (fadeSystem.readyToChangeScene())
		{
			camera.toggleGameViews();
			if (!camera.isTavernView())
			{
				wagon.startWagon();
			}
			else
			{
				terrain.createTrail();
   			wagon.resetWagon();
			}
		}
	}
	
	//**************Draw HUD START*********************

	if(hud.on && !camera.isShadowMapView() && !camera.isFreeRoam())
	{
		glUseProgram(pid);
		glUniform1i(h_flag, 1);
		if(menu.inMenu)
		{
			glUseProgram(pid);
			menu.drawMenu();
			glUseProgram(pid);
		}
		// }
		// if(manager.getInMenu()){
		// 	printf("Menu is up!");
		// 	glUseProgram(pid);
		// 	manager.drawMenuManager();
		// 	glUseProgram(pid);
		// }
		hud.drawHud(h_ModelMatrix, h_vertPos, g_width, g_height, h_aTexCoord);
		glUniform1i(h_flag, 0);

		if(!hud.homeScreenOn)
		{
			char info[64];
			sprintf(info,"x %d", manager.getGold());
			printText2D(info, 50, 566, 18);

			sprintf(info,"x %d", manager.getFood());
			printText2D(info, 220, 566, 18);

			sprintf(info,"x %d", manager.getBeer());
			printText2D(info, 430, 566, 18);

			sprintf(info,"x %d", manager.getMercs());
			printText2D(info, 620, 566, 18);
		}
		else
		{
			printText2D("Press Enter to Continue", 75, 75, 24);
		}
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

	float curCam[6] = {
    camera.bound.minX + camPos.x,
    camera.bound.maxX + camPos.x,
    camera.bound.minY + 1,
    camera.bound.maxY + 1,
    camera.bound.minZ + camPos.z,
    camera.bound.maxZ + camPos.z};

	bool validMove = (curCam[0] < 6.75 || curCam[1] > 39.5 || curCam[4] < -36.0 || curCam[5] > -11.4);

	// for (std::vector<Obj3d>::iterator it1 = objs.begin(); it1 != objs.end(); ++it1)
	// {
	// 	glm::vec3 pos1 = it1 ->getCurSpot();
	// 	if(it1->bound.checkCollision(curCam, it1->scale, pos1))
	// 	{
	// 		validMove = true;
	// 	}
	// }

	int row, col;
  	row = (camPos.x - minX)/gridSize;
 	col = (camPos.z - minZ)/gridSize;
	vector<Obj3d> currentCellObjs = grid[row][col];
	printf("Checking %d collisions!!\n", currentCellObjs.size());

 	// for(int i = 0; i < currentCellObjs.size(); i++)
 	// {
 	// 	vec3 curTrans = currentCellObjs[i].getCurSpot();

 	// 	if (currentCellObjs[i].bound.checkCollision(curCam, currentCellObjs[i].scale, curTrans))
 	// 	{
 	//   		if(!currentCellObjs[i].done)
  //     		{
  //       		currentCellObjs[i].hit();
  //       		printf("Hit item %d at %lf, %lf\n", i, camPos.x, camPos.z);
  //     		}

  //   		validMove = false;
  //   	}
  // 	}

	for(vector<Obj3d>::iterator it1 = currentCellObjs.begin(); it1 != currentCellObjs.end(); ++it1)
	{
		vec3 pos1 = it1 ->getCurSpot();
		if(it1->bound.checkCollision(curCam, it1->scale, pos1))
		{
			validMove = true;
			printf("Hit object at %lf, %lf\n!!", camPos.x, camPos.z);

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
   		terrain.createTrail();
   		wagon.resetWagon();
   	}

	//Print Manager status
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		manager.reportStats();
	}
	
	//Buy food
	if (key == GLFW_KEY_F && action == GLFW_PRESS && camera.isTavernView())
	{
		manager.buyFood(5);
	}

	//Buy beer
	if (key == GLFW_KEY_B && action == GLFW_PRESS && camera.isTavernView())
	{
		manager.buyBeer(2);
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
			manager.buyMercenary(key - GLFW_KEY_1, &tavern);
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
        hud.deadScreenOn = !hud.deadScreenOn;
	}


	if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        tavern.showMercsForSale();
    }

	//Leave Tavern
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
      manager.inTavern = manager.inTavern ? false : true;
		//camera.toggleGameViews();
		audio.playBackgroundMusic(manager.inTavern);

		fadeSystem.startFade(g_width, g_height);
	}

   	//Toggle between lines and filled polygons
   	if (key == GLFW_KEY_L && action == GLFW_PRESS)
   	{
      	GLint polyType;
      	glGetIntegerv(GL_POLYGON_MODE, &polyType);
      	if(polyType == GL_FILL) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			} 
			else 
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
   	}

   	//Toggle culling.
   	if (key == GLFW_KEY_K && action == GLFW_PRESS)
   	{
      	cull = !cull;
      	if(cull) {
				glEnable(GL_CULL_FACE);
			} 
			else 
			{
				glDisable(GL_CULL_FACE);
			}
   	}
   	//Start wagon
   	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		{
			wagon.startWagon();
		}
	//}
		//testing frustum culling
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		fCuller.toggleMode();
		//will be using this to toggle it on and off at a specified points, maybe others too....
	}
	//freezes current projection matrix in for frustum culling
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		fCuller.holdView();
	}
	//Toggle hud
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		hud.on = !hud.on;
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
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
        
    }
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		// fire.toggle();
		tavern.newEmblem();
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		audio.pause();
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		audio.playSoundEffect(EXPLOSION_SOUND);
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		audio.playVoice(BANDIT_GREETING);
	}

	//DO NOT DELETE THE BELOW CODE THAT IS COMMENTED!!!!!!!!!!, OKAY I PROMISE I WONT

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

void checkCollisions(){
	//Check collisions here.
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
	installShaders("lab7_vert.glsl", "lab7_frag.glsl");
	fCuller.init();
	// terrEv.init(&matSetter, &fCuller, &meshes);
	std::string str = "assets/bunny.obj";
	// initShape(&str[0u]); //initShape(argv[0]);

	menu.initMenu(&texLoader, h_ModelMatrix, h_vertPos, g_width, g_height, h_aTexCoord, &manager, &gamePaused);
  	
  	initModels();
  	tavern.loadTavernMeshes(&texLoader);

 	//used only for testing purposes
  	// vec3 loc = terrain.getStartPosition();
  	// terrEv.addMerchantStand(vec3(loc.x - 95.5, loc.y, loc.z), glm::mat4(1.0f));
  	// terrEv.addMerchantStand(vec3(loc.x - 92.5, loc.y, loc.z), glm::rotate(glm::mat4(1.0f), (const float)90, glm::vec3(0, 1.0f, 0)));
  	// terrEv.addMerchantStand(vec3(loc.x - 89.5, loc.y, loc.z), glm::rotate(glm::mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)));
  	// terrEv.addEndCity(vec3(loc.x - 82.5, loc.y, loc.z));

  	//Create about vector and add an element

  	hud.initHUD(&texLoader);
  	hud.initHomeScreen(&texLoader);
  	initText2D( "Holstein.DDS" );
  	dtDraw = 0;
  	audio.playBackgroundMusic(true);

 //  	vector<string> about;
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
    	printf("curPos x: %lf, curPos z: %lf\n", curPos.x, curPos.z);
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

    	printf("maxX: %lf, minX: %lf\n", curBox[1], curBox[0]);

    	exptX = (curBox[1] - curBox[0])/gridSize;
    	loopi = std::max((int)exptX + 1, 1);
    	exptZ = (curBox[5] - curBox[4])/gridSize;
    	loopj = std::max((int)exptZ + 1, 1);

    	printf("exptX: %d, exptZ: %d\n", exptX, exptZ);

    	col = (curBox[0] - minX)/gridSize;
   		row = (curBox[4] - minZ)/gridSize;

    	printf("col: %d, row: %d\n", col, row);

    	// printf("loopi = %d, loopj = %d\n", loopi, loopj);

    	for(int i = col; i < loopi + col; i++)
    	{
      		for(int j = row; j < loopj + row; j++)
      		{
       		  grid[i][j].push_back(objs[k]);
      		  printf("gridsize at %d, %d: %d\n", i, j, grid[i][j].size());
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
			if (camera.isTavernView() && !camera.isFreeRoam())
			{
				checkCollisions();
			}
			updateModels();
			timeOldDraw += (1.0 / 60.0);
			//Draw an image
			drawGL();
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
