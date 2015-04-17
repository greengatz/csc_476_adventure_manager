/**
 * Lab 1
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
#include "MatrixStack.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "RenderingHelper.h"
#include "TextureLoader.h"

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

int NUMOBJ = 5;
Camera camera;
bool cull = false;
bool line = false;
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

/**
 * Helper function to send materials to the shader - create below.
 */
void SetMaterial(int i)
{
	glUseProgram(pid);
	switch(i) {
		case 0: //Red Default
			glUniform3f(h_ka, 0.15, 0.15, 0.15);
			glUniform3f(h_kd, 0.8, 0.2, 0.2);
			glUniform3f(h_ks, 0.2, 0.2, 0.2);
			glUniform1f(h_s, 50.0);
			break;
		case 1: //Green
			glUniform3f(h_ka, 0.15, 0.15, 0.15);
			glUniform3f(h_kd, 0.2, 0.8, 0.2);
			glUniform3f(h_ks, 0.2, 0.2, 0.2);
			glUniform1f(h_s, 50.0);
			break;
		case 2: //Default light
			glUniform3f(h_ka, 0.15, 0.15, 0.15);
			glUniform3f(h_kd, 0.3, 0.3, 0.3);
			glUniform3f(h_ks, 0.2, 0.2, 0.2);
			glUniform1f(h_s, 50.0);
			break;
	}
}

static void initNumPlane() {

   float g_groundSize = 1;

  // A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
    float GrndPos[] = {
    -g_groundSize, -g_groundSize, 0.0,
    -g_groundSize, g_groundSize, 0.0,
     g_groundSize, g_groundSize, 0.0, 
     g_groundSize, -g_groundSize, 0.0
    };

    float GrndNorm[] = {
     0, 0, 1,
     0, 0, 1,
     0, 0, 1,
     0, 0, 1,
     0, 0, 1,
     0, 0, 1
    };


  static GLfloat GrndTex[] = {
      0, 0, // back
      0, 1,
      1, 1,
      1, 0 };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    g_GiboLen = 6;
    glGenBuffers(1, &NumBufObj);
    glBindBuffer(GL_ARRAY_BUFFER, NumBufObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

    glGenBuffers(1, &NumTexBufObj);
    glBindBuffer(GL_ARRAY_BUFFER, NumTexBufObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &NumIndBufObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NumIndBufObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

}

/**
 * For now, this just initializes the Shape object.
 * Later, we'll updated to initialize all objects moving.
 * (This is very specific right now to one object).....
 */
void initShape(char * filename)
{
	t = 0.0f;
	h = 0.001f;
	// g = glm::vec3(0.0f, -0.01f, 0.0f);

	//Load Shape object
	// for (std::vector<Shape>::iterator it = shapes.begin(); it != shapes.end(); ++it){
	// 	it->load(filename);
	// }
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
	//Maybe later have a inheritance of Shape and go
	// through each and init each one.

	//Initialize Shape object
	// for (std::vector<Shape>::iterator it = shapes.begin(); it != shapes.end(); ++it){
	// 	it->init();
	// }

	//Initialize Terrain object
	terrain.init(&texLoader);

	wall.init(&texLoader);

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

	/*Toggle for plane coloring*/
    terrainToggleID = GLSL::getUniformLocation(pid, "terrainToggle");

	// Check GLSL
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

void drawScore()
{
	//set up the texture unit
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, 101);

   GLSL::enableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, NumBufObj);
   glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, NumTexBufObj);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   // Bind index array for drawing
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NumIndBufObj);
   glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_vertPos);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisable(GL_TEXTURE_2D);
}

void drawWalls()
{
	ModelTrans.loadIdentity();
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
	glUseProgram(pid);

	//Update Camera
	// Get mouse position
  	double xpos, ypos;

  	glfwGetCursorPos(window, &xpos, &ypos);
	camera.update(xpos, ypos);

	// Enable backface culling
	if(cull) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(line) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glUniform3fv(h_lightPos1, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform3fv(h_lightPos2, 1, glm::value_ptr(glm::vec3(-1.0f, 1.0f, 1.0f)));
	glUniform1f(h_option, optionS);
	
	// Bind the program
	
	//Set projection matrix
	MatrixStack proj, view;
	proj.pushMatrix();
	camera.applyProjectionMatrix(&proj);
	glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	proj.pushMatrix();
	camera.applyViewMatrix(&view);
	glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));

	bool winCondition = true;
	// Draw shapes
	for (std::vector<Shape>::iterator it = shapes.begin(); it != shapes.end(); ++it){
    // std::cout << ' ' << ;
	// for(int i = 0; i < shapeCount; i++){
		if(it->isGreen())
			SetMaterial(1);
		else{
			winCondition = false;
			SetMaterial(0);
		}
		ModelTrans.loadIdentity();
		ModelTrans.pushMatrix();
		ModelTrans.translate(it->getPosition());
		glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
		ModelTrans.popMatrix();
		it->draw(h_vertPos, h_vertNor);	
	}

	if(winCondition){
		//reset bunnies & increase the count
		printf("You won!\n");
		points = 0;
		printf("You have %d points!\n", points);
		shapes.clear();
		NUMOBJ += 10;
	}

	SetMaterial(2);
	ModelTrans.loadIdentity();
	ModelTrans.pushMatrix();
	glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
	//Still need to set position for terrain.
	ModelTrans.popMatrix();

	glUniform1i(terrainToggleID, 1);
	glUniform1i(h_uTexUnit, 0);
	terrain.draw(h_vertPos, h_vertNor, h_aTexCoord);


	ModelTrans.loadIdentity();
	ModelTrans.pushMatrix();
		ModelTrans.translate(glm::vec3(20, 0, -20));
		ModelTrans.scale(200.0, 200.0, 200.0);
		glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
		ModelTrans.popMatrix();
	drawScore();


	drawWalls();
	glUniform1i(terrainToggleID, 0);
	
	// Unbind the program
	glUseProgram(0);

	// Pop stacks
	proj.popMatrix();
}

bool hasCollided(glm::vec3 incr)
{
	for (std::vector<Shape>::iterator it1 = shapes.begin(); it1 != shapes.end(); ++it1)
	{
		glm::vec3 pos1 = it1 ->getPosition();
		glm::vec3 camPos = camera.getPosition() + incr;
		float d = sqrt(((pos1.x - camPos.x) * (pos1.x - camPos.x)) + ((pos1.z - camPos.z) * (pos1.z - camPos.z)));

		if (d <= it1->getRadius() * 2)
		{
			it1->freezeShape();
			if (!it1->isGreen())
			{
				printf("You have %d points!\n", ++points);
			}
			it1->setColorGreen();
			return true;
		}
	}
	return false;
}

/**
 * This will get called when any button on keyboard is pressed.
 */

void checkUserInput()
{
   vec3 view = camera.getLookAtPoint() - camera.getTheEye();
   vec3 strafe = glm::cross(view, vec3(0.0, 1.0, 0.0));
   if (glfwGetKey(window, GLFW_KEY_C ) == GLFW_PRESS)
   {
      cull = !cull;
   }
   if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
   {
      line = !line;
   }
   if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS && !hasCollided(-strafe))
   {
      //theStrafe -= strafe * strafeSpeed;
      camera.updateStrafe(-strafe);
   }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !hasCollided(strafe))
   {
      //theStrafe += strafe * strafeSpeed;
      camera.updateStrafe(strafe);
   }
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !hasCollided(view*1.2f))
   {
      //theZoom += view * sprintSpeed;
      camera.updateZoom(view*1.2f);
   }
   else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !hasCollided(view))
   {
      //theZoom += view * walkSpeed;
      camera.updateZoom(view);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !hasCollided(-view*1.2f))
   {
      //theZoom -= view * sprintSpeed;
      camera.updateZoom(-view*1.2f);
   }
   else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !hasCollided(-view))
   {
      //theZoom -= view * walkSpeed;
      camera.updateZoom(-view);
   }
}

void window_size_callback(GLFWwindow* window, int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	g_width = w;
	g_height = h;
	camera.setWindowSize(w, h);
}

void checkCollisions(){
	for (std::vector<Shape>::iterator it1 = shapes.begin(); it1 != shapes.end(); ++it1){
		glm::vec3 pos1 = it1->getPosition();
		for (std::vector<Shape>::iterator it2 = shapes.begin(); it2 != shapes.end(); ++it2){
			glm::vec3 pos2 = it2->getPosition();
			float d = sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x)) + ((pos1.z - pos2.z) * (pos1.z - pos2.z)));
			int index1 = std::distance(shapes.begin(), it1);
			int index2 = std::distance(shapes.begin(), it2);
			if(index1 != index2 && d <= it1->getRadius() + it2->getRadius()){
				it1->freezeShape();
				it2->freezeShape();
			}
		}
	}

	for (std::vector<Shape>::iterator it1 = shapes.begin(); it1 != shapes.end(); ++it1)
	{
		glm::vec3 pos1 = it1 ->getPosition();
		glm::vec3 camPos = camera.getPosition();
		float d = sqrt(((pos1.x - camPos.x) * (pos1.x - camPos.x)) + ((pos1.z - camPos.z) * (pos1.z - camPos.z)));

		if (d <= it1->getRadius() * 2)
		{
			it1->freezeShape();
			if (!it1->isGreen())
			{
				printf("You have %d points!\n", ++points);
			}
			it1->setColorGreen();
		}
	}
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
   window = glfwCreateWindow( g_width, g_height, "CPE476 Lab1 [ Clark | Harper ]", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    //glfwSetKeyCallback(window, key_callback);

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
	std::string str = "assets/bunny.obj";
	// initShape(&str[0u]); //initShape(argv[0]);
  	initModels();
   do{
   	timeNew = glfwGetTime();
	double dtSpawn = timeNew - timeOldSpawn;

	// Update every 1s
	if(shapes.size() != NUMOBJ && dtSpawn >= timeOldSpawn) {
		float randomX = rF(10.0f, 40.0f);
		float randomZ = rF(-10.0f, -40.0f);
		for (std::vector<Shape>::iterator it = shapes.begin(); it != shapes.end(); ++it){
			glm::vec3 temp = it->getPosition();
			if((temp.x < randomX + 1 && temp.x > randomX - 1) || 
				(temp.z < randomZ + 1 && temp.z > randomZ - 1)){
				randomX = rF(10.0f, 40.0f);
				randomZ = rF(-10.0f, -40.0f);
				it = shapes.begin();
			}
		}
		spinOffNewShape(&str[0u], randomX, randomZ);
		timeOldSpawn += 1.0;
	}
	
	
   	//Check for user input
   	//checkUserInput();
		double dtDraw = timeNew - timeOldDraw;
		t += h;
		// Update every 60Hz
		if(dtDraw >= (1.0 / 60) ) {
			checkUserInput();
			checkCollisions();
			timeOldDraw += (1.0 / 60);
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
