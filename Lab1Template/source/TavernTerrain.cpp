/**
 * Terrain.cpp - Terrain object
 * @author Brandon Clark
 */

#include <cstdlib>
#include <time.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "TavernTerrain.h"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// #define GRASS 3
// #define TRAIL 0
// #define RED 1
// #define LBTRAIL 2
// #define RBTRAIL 4
// #define LTTRAIL 5
// #define RTTRAIL 6

using namespace std;

int TERRAIN_TEX_TAVERNFLOOR_ID = 5200;

TavernTerrain::TavernTerrain() :
	x(0.0f, 0.0f, 0.0f),
	scale(5.0f),
	posBufID(0),
	norBufID(0),
	texBufID(0)
{
}

TavernTerrain::~TavernTerrain()
{
}

void TavernTerrain::init(TextureLoader* texLoader)
{
	x.x = -10.0f;
	x.y = 0.0f;
	x.z = 10.0f;
	scale = 0.75f;
	GLfloat terrain_buffer[(MAP_Z - 1) * (MAP_X - 1) * 12];
  	GLfloat terrain_norm[(MAP_Z - 1) * (MAP_X - 1) * 12];
  	GLfloat terrain_tex[(MAP_Z - 1) * (MAP_X - 1) * 8];

  	int index = 0;
  	// loop through all of the heightfield points, calculating
  	// the coordinates for each point
  	for (int z = 0; z < MAP_Z; z++)
  	{
    	for (int x = 0; x < MAP_X; x++, index++)
    	{
      	terrain[x][z][0] = float(x) * MAP_SCALE;    
      	terrain[x][z][1] = 0; //restrict y to 0
      	terrain[x][z][2] = -float(z) * MAP_SCALE;
    	}
  	}

  	//initialize normals to zero
  	for (int x = 0; x < (MAP_Z - 1) * (MAP_X - 1) * 12; x++)
  	{
    	terrain_buffer[x] = 0;
    	terrain_norm[x] = 0;
  	}

  	index = 0;
  	for (int z = 0; z < MAP_Z - 1; z++)
  	{	
    	for (int x = 0; x < MAP_X - 1; x++, index++)
    	{

      	//Calculate normal
      	glm::vec3 v1 = glm::vec3(terrain[x][z][0], terrain[x][z][1], terrain[x][z][2]);
      	glm::vec3 v2 = glm::vec3(terrain[x+1][z][0], terrain[x+1][z][1], terrain[x+1][z][2]);
      	glm::vec3 v3 = glm::vec3(terrain[x][z+1][0], terrain[x][z+1][1], terrain[x][z+1][2]);
      	glm::vec3 nor = glm::cross(v2-v1, v3-v1);
      
      	//v1
      	terrain_buffer[12 * index + 0] = terrain[x][z][0]; //x
      	terrain_buffer[12 * index + 1] = terrain[x][z][1]; //y
      	terrain_buffer[12 * index + 2] = terrain[x][z][2]; //z
      	//add normal to sum
      	terrain_norm[12 * index + 0] += nor.x;
      	terrain_norm[12 * index + 1] += nor.y;
      	terrain_norm[12 * index + 2] += nor.z;
      	//set texture coordinate for that point
      	terrain_tex[8 * index + 0] = 0.0;
      	terrain_tex[8 * index + 1] = 0.0;

      	//v2
      	terrain_buffer[12 * index + 3] = terrain[x+1][z][0];
      	terrain_buffer[12 * index + 4] = terrain[x+1][z][1];
      	terrain_buffer[12 * index + 5] = terrain[x+1][z][2];
      	terrain_norm[12 * index + 3] += nor.x;
      	terrain_norm[12 * index + 4] += nor.y;
      	terrain_norm[12 * index + 5] += nor.z;
      	terrain_tex[8 * index + 2] = 1.0;
      	terrain_tex[8 * index + 3] = 0.0;

      	//v3
      	terrain_buffer[12 * index + 6] = terrain[x][z+1][0];
      	terrain_buffer[12 * index + 7] = terrain[x][z+1][1];
      	terrain_buffer[12 * index + 8] = terrain[x][z+1][2];
      	terrain_norm[12 * index + 6] += nor.x;
      	terrain_norm[12 * index + 7] += nor.y;
      	terrain_norm[12 * index + 8] += nor.z;
      	terrain_tex[8 * index + 4] = 0.0;
      	terrain_tex[8 * index + 5] = 1.0;

      	//v4
      	terrain_buffer[12 * index + 9] = terrain[x+1][z+1][0];
      	terrain_buffer[12 * index + 10] = terrain[x+1][z+1][1];
      	terrain_buffer[12 * index + 11] = terrain[x+1][z+1][2];
      	terrain_norm[12 * index + 9] += nor.x;
      	terrain_norm[12 * index + 10] += nor.y;
      	terrain_norm[12 * index + 11] += nor.z;
      	terrain_tex[8 * index + 6] = 1.0;
      	terrain_tex[8 * index + 7] = 1.0;
    	}
  	}

  	//normalize the normals
  	for (int x = 0; x < ((MAP_Z - 1) * (MAP_X - 1) * 12)/3; x += 3)
  	{
    	glm::vec3 nor = glm::normalize(glm::vec3(terrain_norm[x], terrain_norm[x+1], 
      terrain_norm[x+2]));

    	terrain_norm[x] = nor.x;
    	terrain_norm[x+1] = nor.y;
    	terrain_norm[x+2] = nor.z;
  	}

  	glGenBuffers(1, &posBufID);
  	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_buffer), terrain_buffer, GL_STATIC_DRAW);
  
  	glGenBuffers(1, &norBufID);
  	glBindBuffer(GL_ARRAY_BUFFER, norBufID);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_norm), terrain_norm, GL_STATIC_DRAW);
  
  	glGenBuffers(1, &texBufID);
  	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_tex), terrain_tex, GL_STATIC_DRAW);

    //Load Texture
    texLoader->LoadTexture((char *)"assets/tavern/tavernFloor.bmp", TERRAIN_TEX_TAVERNFLOOR_ID);
    // texLoader->LoadTexture((char *)"assets/grass2.bmp", TERRAIN_TEX_ID);
    // texLoader->LoadTexture((char *)"assets/trail2.bmp", TERRAIN_TEX_DIRT_ID);
    // texLoader->LoadTexture((char *)"assets/trailtopleft.bmp", TERRAIN_TEX_LBTRAIL_ID);
    // texLoader->LoadTexture((char *)"assets/trailbottomleft.bmp", TERRAIN_TEX_RBTRAIL_ID);
    // texLoader->LoadTexture((char *)"assets/trailbottomright.bmp", TERRAIN_TEX_RTTRAIL_ID);
    // texLoader->LoadTexture((char *)"assets/trailtopright.bmp", TERRAIN_TEX_LTTRAIL_ID);
    // texLoader->LoadTexture((char *)"assets/red.bmp", TERRAIN_TEX_RED_ID);

  	//unbind the arrays
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	assert(glGetError() == GL_NO_ERROR);
}

void TavernTerrain::draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, RenderingHelper *modelTrans)
{

   //Position Wagon along the trail
   modelTrans->pushMatrix();
      modelTrans->translate(x);
      modelTrans->rotate(0.0f, glm::vec3(0, 1, 0));
      modelTrans->scale(scale, scale, scale);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(modelTrans->modelViewMatrix));
   modelTrans->popMatrix();

  //set up the texture unit
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    // //glUniform1i(h_uTexUnit, 0);
    // for (int n = 0; n < MAP_Z; n++)
    // {
    //     for (int m = 0; m < MAP_Z; m++)
    //     {
    //         if(trailMap[n][z] == GRASS){
    //             glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_ID);


    //         }
    //     }
    // }
    // glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_ID);

	// Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_nor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufID);
   glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_pos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, texBufID);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_TAVERNFLOOR_ID);

   int size = 0;
   for (int index = 0; index < MAP_X - 1; index++)
   {
      glDrawArrays(GL_TRIANGLE_STRIP, size, (MAP_X - 1) * 4);
      size += (MAP_X - 1) * 4;
   }

   GLSL::disableVertexAttribArray(h_pos);
   GLSL::disableVertexAttribArray(h_nor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisable(GL_TEXTURE_2D);
}
