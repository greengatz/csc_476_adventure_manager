/**
 * Mercenary Manager
 * @author Brandon Clark
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "GLSL.h"
#include "Camera.h"
#include "Shape.h"
#include "Terrain.h"
#include "Wall.h"
#include "hud.h"
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

using namespace std;

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

Wall wall;

Wagon wagon;

int NUMOBJ = 5;
Camera camera;
bool cull = false;
glm::vec2 mouse;
int shapeCount = 1;
std::vector<Shape> shapes;
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
GLint h_hudColor;

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
int g_GiboLen;
GLuint NumBufObj, NumIndBufObj, NumTexBufObj;

//Rendering Helper
RenderingHelper ModelTrans;
Tavern tavern;
Manager manager("The Dude");
TavernTerrain tavTerr;
Materials matSetter;
FrustumCull fCuller;
HUD hud(&manager);

/**
 * Helper function to send materials to the shader - create below.
 */
// void SetMaterial(int i)
// {
// 	glUseProgram(pid);
// 	switch(i) {
// 		case 0: //Red Default
// 			glUniform3f(h_ka, 0.15, 0.15, 0.15);
// 			glUniform3f(h_kd, 0.8, 0.2, 0.2);
// 			glUniform3f(h_ks, 0.2, 0.2, 0.2);
// 			glUniform1f(h_s, 50.0);
// 			break;
// 		case 1: //Green
// 			glUniform3f(h_ka, 0.15, 0.15, 0.15);
// 			glUniform3f(h_kd, 0.2, 0.8, 0.2);
// 			glUniform3f(h_ks, 0.2, 0.2, 0.2);
// 			glUniform1f(h_s, 50.0);
// 			break;
// 		case 2: //Default light
// 			glUniform3f(h_ka, 0.15, 0.15, 0.15);
// 			glUniform3f(h_kd, 0.3, 0.3, 0.3);
// 			glUniform3f(h_ks, 0.2, 0.2, 0.2);
// 			glUniform1f(h_s, 50.0);
// 			break;
// 		case 3: //Wood
// 			glUniform3f(h_ka, 0.35, 0.35, 0.35);
// 			glUniform3f(h_kd, 0.804, 0.666, 0.49);
// 			glUniform3f(h_ks, 0.1, 0.1, 0.1);
// 			glUniform1f(h_s, 0.01);
// 			break;
// 		case 4: //not specular Wood
// 			glUniform3f(h_ka, 0.1, 0.1, 0.1);
// 			glUniform3f(h_kd, 0.404, 0.366, 0.29);
// 			glUniform3f(h_ks, 0.051, 0.051, 0.051);
// 			glUniform1f(h_s, 0.01);
// 			break;
// 	}
// }

// static void initNumPlane() {

//    float g_groundSize = 1;

//   // A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
//     float GrndPos[] = {
//     -g_groundSize, -g_groundSize, 0.0,
//     -g_groundSize, g_groundSize, 0.0,
//      g_groundSize, g_groundSize, 0.0, 
//      g_groundSize, -g_groundSize, 0.0
//     };

//     float GrndNorm[] = {
//      0, 0, 1,
//      0, 0, 1,
//      0, 0, 1,
//      0, 0, 1,
//      0, 0, 1,
//      0, 0, 1
//     };


//   static GLfloat GrndTex[] = {
//       0, 0, // back
//       0, 1,
//       1, 1,
//       1, 0 };

//     unsigned short idx[] = {0, 1, 2, 0, 2, 3};

//     g_GiboLen = 6;
//     glGenBuffers(1, &NumBufObj);
//     glBindBuffer(GL_ARRAY_BUFFER, NumBufObj);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

//     glGenBuffers(1, &NumTexBufObj);
//     glBindBuffer(GL_ARRAY_BUFFER, NumTexBufObj);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

//     glGenBuffers(1, &NumIndBufObj);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NumIndBufObj);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

// }

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

/**
 * Generalized approach to intialization.
 */

void spinOffNewShape(char * filename, float x, float z){
	Shape temp;
	temp.load(filename);
	temp.init(x, z);
	shapes.push_back(temp);
}

void initModels()
{
	//Initialize Terrain object
	terrain.init(&texLoader);
	tavTerr.init(&texLoader);

	//Initalize Wall
	wall.init(&texLoader);

	//Initalize Wagon
	wagon.init(&texLoader, &terrain);

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
	h_hudColor = GLSL::getUniformLocation(pid, "hudColor");

	/*Toggle for plane coloring*/
    terrainToggleID = GLSL::getUniformLocation(pid, "terrainToggle");

    matSetter.init(&pid, &h_ka, &h_kd, &h_ks, &h_s);

	// Check GLSL
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

//This will be in wall class eventually....
void drawWalls()
{
	ModelTrans.pushMatrix();
		ModelTrans.translate(glm::vec3(3.0, 0.0, 0.0));
		glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
		wall.draw(h_vertPos, h_vertNor, h_aTexCoord);
		for (int i = 0; i < 7; i++)
		{
			ModelTrans.translate(glm::vec3(5.7, 0.0, 0.0));
			glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
			wall.draw(h_vertPos, h_vertNor, h_aTexCoord);
		}
		ModelTrans.translate(glm::vec3(3.3, 0.0, 0.0));
		glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
		wall.draw(h_vertPos, h_vertNor, h_aTexCoord);
		for (int j = 0; j < 3; j++)
		{
			ModelTrans.translate(glm::vec3(2.8, 0.0, 0.0));
			ModelTrans.pushMatrix();
				ModelTrans.rotate(90.0, glm::vec3(0, 1, 0));
				ModelTrans.translate(vec3(-3.0, 0.0, 0.0));
				for (int i = 0; i < 8; i++)
				{
					ModelTrans.translate(glm::vec3(5.7, 0.0, 0.0));
					glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
					wall.draw(h_vertPos, h_vertNor, h_aTexCoord);
				}
				ModelTrans.translate(glm::vec3(3.3, 0.0, 0.0));
				glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
				wall.draw(h_vertPos, h_vertNor, h_aTexCoord);
		}
					ModelTrans.popMatrix();
				ModelTrans.popMatrix();
			ModelTrans.popMatrix();
	ModelTrans.popMatrix();
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

	//========================== DRAW OUTSIDE SCENE ====================
	if (!camera.isTavernView() || camera.isFreeRoam())
	{
		glUniform1i(terrainToggleID, 1);
		glUniform1i(h_uTexUnit, 0);
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
			ModelTrans.translate(glm::vec3(-100.0, 0.0, 0.0));
			glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
			//ModelTrans.popMatrix();
			ModelTrans.pushMatrix();
				terrain.draw(h_vertPos, h_vertNor, h_aTexCoord);
				drawWalls();
				wagon.draw(h_vertPos, h_vertNor, h_aTexCoord, h_ModelMatrix, &ModelTrans);
			ModelTrans.popMatrix();
		ModelTrans.popMatrix();
		glUniform1i(terrainToggleID, 0);
	}

	//========================= END OUTSIDE SCENE =======================

	if (camera.isTavernView() || camera.isFreeRoam())
	{
		//Draw TAVERN
		glUniform1i(terrainToggleID, 1);
		glUniform1i(h_uTexUnit, 0);
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		tavTerr.draw(h_vertPos, h_vertNor, h_aTexCoord, h_ModelMatrix, &ModelTrans);
		matSetter.setMaterial(4);
		ModelTrans.popMatrix();
		matSetter.setMaterial(3);
		tavern.drawTavern(h_ModelMatrix, h_vertPos, h_vertNor, h_aTexCoord);
		glUniform1i(terrainToggleID, 0);
	}

	//**************Draw HUD START*********************
	glUniform1i(h_flag, 1);
	hud.drawHud(h_ModelMatrix, h_vertPos, h_hudColor, g_width, g_width);

	//**************Draw HUD FINISH********************
	
	// Unbind the program
	glUseProgram(0);
	// Pop stacks
	proj.popMatrix();
}

bool hasCollided(glm::vec3 incr)
{
	vector<Obj3d> temp = tavern.tavernItems;
	glm::vec3 camPos = camera.getPosition() + incr;

	float curCam[6] = {
    camera.bound.minX + camPos.x,
    camera.bound.maxX + camPos.x,
    camera.bound.minY + 1,
    camera.bound.maxY + 1,
    camera.bound.minZ + camPos.z,
    camera.bound.maxZ + camPos.z};

	bool validMove = (curCam[0] < 6.75 || curCam[1] > 39.5 || curCam[4] < -36.0 || curCam[5] > -11.4);

	for (std::vector<Obj3d>::iterator it1 = temp.begin(); it1 != temp.end(); ++it1)
	{
		glm::vec3 pos1 = it1 ->getCurSpot();
		if(it1->bound.checkCollision(curCam, it1->scale, pos1))
		{
			validMove = true;
		}
	}

	if (camera.isFreeRoam())
	{
		return false;
	}
	else
	{
		return validMove;
	}
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
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		manager.buyFood();
	}

	//Buy beer
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		manager.buyBeer();
	}
	
	if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5 && action == GLFW_PRESS)
	{
		// tavern.buyMercenary(key - GLFW_KEY_1, &manager);
		manager.buyMercenary(key - GLFW_KEY_1, &tavern);
	}
	
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		//manager.buyMercenary(key - GLFW_KEY_1, &tavern);
        tavern.tavernCharacters[0].wave();
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        tavern.showMercsForSale();
    }

	//Leave Tavern
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
        manager.inTavern = manager.inTavern ? false : true;
		camera.toggleGameViews();
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
	wagon.updateWagon(t);
}

void checkCollisions(){
	//Check collisions here.
}

int main(int argc, char **argv)
{
	// Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
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
	tavern.init(&matSetter, &fCuller);
	std::string str = "assets/bunny.obj";
	// initShape(&str[0u]); //initShape(argv[0]);
  	initModels();
  	tavern.loadTavernMeshes(&texLoader);
  	hud.initHUD();
   do{
   	timeNew = glfwGetTime();
	
		double dtDraw = timeNew - timeOldDraw;
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
