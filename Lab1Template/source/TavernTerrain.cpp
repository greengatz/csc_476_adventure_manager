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

using namespace std;

int TERRAIN_TEX_TAVERN_FLOOR_ID = 5200;
int TERRAIN_TEX_TAVERN_WALL_ID = 5400;

TavernTerrain::TavernTerrain() :
	ground(0.0f, 0.0f, 0.0f),
  wall1(0.0f, 0.0f, 0.0f),
  wall2(0.0f, 0.0f, 0.0f),
	scale(5.0f),
	posBufID(0),
	norBufID(0),
	texBufID(0)
{
}

TavernTerrain::~TavernTerrain()
{
}

void TavernTerrain::loadWalls()
{
    //this is the first wall
    GLfloat wall_buffer[(MAP_Y - 1) * (MAP_X - 1) * 12];
    GLfloat wall_norm[(MAP_Y - 1) * (MAP_X - 1) * 12];
    GLfloat wall_tex[(MAP_Y - 1) * (MAP_X - 1) * 8];

      int index = 0;
    // loop through all of the heightfield points, calculating
    // the coordinates for each point
    for (int y = 0; y < MAP_Y; y++)
    {
      for (int x = 0; x < MAP_X; x++, index++)
      {
        wallData[x][y][0] = float(x) * MAP_SCALE;    
        wallData[x][y][1] = -float(y) * MAP_SCALE;
        wallData[x][y][2] = 0; //restrict z to 0
      }
    }

    //initialize normals to zero
    for (int x = 0; x < (MAP_Y - 1) * (MAP_X - 1) * 12; x++)
    {
      wall_buffer[x] = 0;
      wall_norm[x] = 0;
    }

    index = 0;
    for (int y = 0; y < MAP_Y - 1; y++)
    { 
      for (int x = 0; x < MAP_X - 1; x++, index++)
      {

        //Calculate normal
        glm::vec3 v1 = glm::vec3(wallData[x][y][0], wallData[x][y][1], wallData[x][y][2]);
        glm::vec3 v2 = glm::vec3(wallData[x+1][y][0], wallData[x+1][y][1], wallData[x+1][y][2]);
        glm::vec3 v3 = glm::vec3(wallData[x][y+1][0], wallData[x][y+1][1], wallData[x][y+1][2]);
        glm::vec3 nor = glm::cross(v2-v1, v3-v1);
      
        //v1
        wall_buffer[12 * index + 0] = wallData[x][y][0]; //x
        wall_buffer[12 * index + 1] = wallData[x][y][1]; //y
        wall_buffer[12 * index + 2] = wallData[x][y][2]; //z
        //add normal to sum
        wall_norm[12 * index + 0] += nor.x;
        wall_norm[12 * index + 1] += nor.y;
        wall_norm[12 * index + 2] += nor.z;
        //set texture coordinate for that point
        wall_tex[8 * index + 0] = 0.0;
        wall_tex[8 * index + 1] = 0.0;

        //v2
        wall_buffer[12 * index + 3] = wallData[x+1][y][0];
        wall_buffer[12 * index + 4] = wallData[x+1][y][1];
        wall_buffer[12 * index + 5] = wallData[x+1][y][2];
        wall_norm[12 * index + 3] += nor.x;
        wall_norm[12 * index + 4] += nor.y;
        wall_norm[12 * index + 5] += nor.z;
        wall_tex[8 * index + 2] = 1.0;
        wall_tex[8 * index + 3] = 0.0;

        //v3
        wall_buffer[12 * index + 6] = wallData[x][y+1][0];
        wall_buffer[12 * index + 7] = wallData[x][y+1][1];
        wall_buffer[12 * index + 8] = wallData[x][y+1][2];
        wall_norm[12 * index + 6] += nor.x;
        wall_norm[12 * index + 7] += nor.y;
        wall_norm[12 * index + 8] += nor.z;
        wall_tex[8 * index + 4] = 0.0;
        wall_tex[8 * index + 5] = 1.0;

        //v4
        wall_buffer[12 * index + 9] = wallData[x+1][y+1][0];
        wall_buffer[12 * index + 10] = wallData[x+1][y+1][1];
        wall_buffer[12 * index + 11] = wallData[x+1][y+1][2];
        wall_norm[12 * index + 9] += nor.x;
        wall_norm[12 * index + 10] += nor.y;
        wall_norm[12 * index + 11] += nor.z;
        wall_tex[8 * index + 6] = 1.0;
        wall_tex[8 * index + 7] = 1.0;
      }
    }

    //normalize the normals
    for (int x = 0; x < ((MAP_Y - 1) * (MAP_X - 1) * 12)/3; x += 3)
    {
      glm::vec3 nor = glm::normalize(glm::vec3(wall_norm[x], wall_norm[x+1], 
      wall_norm[x+2]));

      wall_norm[x] = nor.x;
      wall_norm[x+1] = nor.y;
      wall_norm[x+2] = nor.z;
    }

    glGenBuffers(1, &wallPosBufID);
    glBindBuffer(GL_ARRAY_BUFFER, wallPosBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_buffer), wall_buffer, GL_STATIC_DRAW);
  
    glGenBuffers(1, &wallNorBufID);
    glBindBuffer(GL_ARRAY_BUFFER, wallNorBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_norm), wall_norm, GL_STATIC_DRAW);
  
    glGenBuffers(1, &wallTexBufID);
    glBindBuffer(GL_ARRAY_BUFFER, wallTexBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_tex), wall_tex, GL_STATIC_DRAW);

    //this is the second wall
    GLfloat wall_buffer2[(MAP_Y - 1) * (MAP_Z - 1) * 12];
    GLfloat wall_norm2[(MAP_Y - 1) * (MAP_Z - 1) * 12];
    GLfloat wall_tex2[(MAP_Y - 1) * (MAP_Z - 1) * 8];

    index = 0;
    // loop through all of the heightfield points, calculating
    // the coordinates for each point
    for (int y = 0; y < MAP_Y; y++)
    {
      for (int z = 0; z < MAP_Z; z++, index++)
      {
        wallData2[z][y][0] =  0; //restrict z to 0   
        wallData2[z][y][1] = -float(y) * MAP_SCALE;
        wallData2[z][y][2] = float(z) * MAP_SCALE;
      }
    }

    //initialize normals to zero
    for (int z = 0; z < (MAP_Y - 1) * (MAP_Z - 1) * 12; z++)
    {
      wall_buffer2[z] = 0;
      wall_norm2[z] = 0;
    }

    index = 0;
    for (int y = 0; y < MAP_Y - 1; y++)
    { 
      for (int z = 0; z < MAP_Z - 1; z++, index++)
      {

        //Calculate normal
        glm::vec3 v1 = glm::vec3(wallData2[z][y][0], wallData2[z][y][1], wallData2[z][y][2]);
        glm::vec3 v2 = glm::vec3(wallData2[z+1][y][0], wallData2[z+1][y][1], wallData2[z+1][y][2]);
        glm::vec3 v3 = glm::vec3(wallData2[z][y+1][0], wallData2[z][y+1][1], wallData2[z][y+1][2]);
        glm::vec3 nor = glm::cross(v2-v1, v3-v1);
      
        //v1
        wall_buffer2[12 * index + 0] = wallData2[z][y][0]; //x
        wall_buffer2[12 * index + 1] = wallData2[z][y][1]; //y
        wall_buffer2[12 * index + 2] = wallData2[z][y][2]; //z
        //add normal to sum
        wall_norm2[12 * index + 0] += nor.x;
        wall_norm2[12 * index + 1] += nor.y;
        wall_norm2[12 * index + 2] += nor.z;
        //set texture coordinate for that point
        wall_tex2[8 * index + 0] = 0.0;
        wall_tex2[8 * index + 1] = 0.0;

        //v2
        wall_buffer2[12 * index + 3] = wallData2[z+1][y][0];
        wall_buffer2[12 * index + 4] = wallData2[z+1][y][1];
        wall_buffer2[12 * index + 5] = wallData2[z+1][y][2];
        wall_norm2[12 * index + 3] += nor.x;
        wall_norm2[12 * index + 4] += nor.y;
        wall_norm2[12 * index + 5] += nor.z;
        wall_tex2[8 * index + 2] = 1.0;
        wall_tex2[8 * index + 3] = 0.0;

        //v3
        wall_buffer2[12 * index + 6] = wallData2[z][y+1][0];
        wall_buffer2[12 * index + 7] = wallData2[z][y+1][1];
        wall_buffer2[12 * index + 8] = wallData2[z][y+1][2];
        wall_norm2[12 * index + 6] += nor.x;
        wall_norm2[12 * index + 7] += nor.y;
        wall_norm2[12 * index + 8] += nor.z;
        wall_tex2[8 * index + 4] = 0.0;
        wall_tex2[8 * index + 5] = 1.0;

        //v4
        wall_buffer2[12 * index + 9] = wallData2[z+1][y+1][0];
        wall_buffer2[12 * index + 10] = wallData2[z+1][y+1][1];
        wall_buffer2[12 * index + 11] = wallData2[z+1][y+1][2];
        wall_norm2[12 * index + 9] += nor.x;
        wall_norm2[12 * index + 10] += nor.y;
        wall_norm2[12 * index + 11] += nor.z;
        wall_tex2[8 * index + 6] = 1.0;
        wall_tex2[8 * index + 7] = 1.0;
      }
    }

    //normalize the normals
    for (int z = 0; z < ((MAP_Y - 1) * (MAP_Z- 1) * 12)/3; z += 3)
    {
      glm::vec3 nor = glm::normalize(glm::vec3(wall_norm2[z], wall_norm2[z+1], 
      wall_norm2[z+2]));

      wall_norm2[z] = nor.x;
      wall_norm2[z+1] = nor.y;
      wall_norm2[z+2] = nor.z;
    }

    glGenBuffers(1, &wall2PosBufID);
    glBindBuffer(GL_ARRAY_BUFFER, wall2PosBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_buffer2), wall_buffer2, GL_STATIC_DRAW);
  
    glGenBuffers(1, &wall2NorBufID);
    glBindBuffer(GL_ARRAY_BUFFER, wall2NorBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_norm2), wall_norm2, GL_STATIC_DRAW);
  
    glGenBuffers(1, &wall2TexBufID);
    glBindBuffer(GL_ARRAY_BUFFER, wall2TexBufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_tex2), wall_tex2, GL_STATIC_DRAW);
}

void TavernTerrain::init(TextureLoader* texLoader)
{
  ground = glm::vec3(5.0f, 0.0f, -5.0f);
  wall1 = glm::vec3(5.0f, 20.0f, -35.0f);
  wall2 = glm::vec3(7.5f, 20.0f, -35.0f);
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

    loadWalls();
    //Load Texture
    texLoader->LoadTexture((char *)"assets/tavern/stonefloor.bmp", TERRAIN_TEX_TAVERN_FLOOR_ID);
    texLoader->LoadTexture((char *)"assets/tavern/tavernFloor.bmp", TERRAIN_TEX_TAVERN_WALL_ID);

  	//unbind the arrays
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	assert(glGetError() == GL_NO_ERROR);
}

void TavernTerrain::setUpStack(RenderingHelper *modelTrans, GLint h_ModelMatrix, glm::vec3 target)
{
   modelTrans->pushMatrix();
      modelTrans->translate(target);
      modelTrans->rotate(0.0f, glm::vec3(0, 1, 0));
      modelTrans->scale(scale, scale, scale);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(modelTrans->modelViewMatrix));
   modelTrans->popMatrix();
}

void TavernTerrain::drawATex(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, 
                             GLuint posBuffer, GLuint norBuffer, GLuint texBuffer, int targetTex)
{
   GLSL::enableVertexAttribArray(h_nor);
   glBindBuffer(GL_ARRAY_BUFFER, norBuffer);
   glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_pos);
   glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
   glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   glBindTexture(GL_TEXTURE_2D, targetTex);

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

void TavernTerrain::draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, RenderingHelper *modelTrans)
{

  setUpStack(modelTrans, h_ModelMatrix, ground);
  drawATex(h_pos, h_nor, h_aTexCoord, h_ModelMatrix, posBufID, norBufID, texBufID, TERRAIN_TEX_TAVERN_FLOOR_ID);

  //draw one of the walls
  setUpStack(modelTrans, h_ModelMatrix, wall1);
  drawATex(h_pos, h_nor, h_aTexCoord, h_ModelMatrix, wallPosBufID, wallNorBufID, wallTexBufID, TERRAIN_TEX_TAVERN_WALL_ID);

  setUpStack(modelTrans, h_ModelMatrix, glm::vec3(wall1.x, wall1.y, -12.0f));
  drawATex(h_pos, h_nor, h_aTexCoord, h_ModelMatrix, wallPosBufID, wallNorBufID, wallTexBufID, TERRAIN_TEX_TAVERN_WALL_ID);

  setUpStack(modelTrans, h_ModelMatrix, wall2);
  drawATex(h_pos, h_nor, h_aTexCoord, h_ModelMatrix, wall2PosBufID, wall2NorBufID, wall2TexBufID, TERRAIN_TEX_TAVERN_WALL_ID);

  setUpStack(modelTrans, h_ModelMatrix, glm::vec3(38.5f, wall2.y, wall2.z));
  drawATex(h_pos, h_nor, h_aTexCoord, h_ModelMatrix, wall2PosBufID, wall2NorBufID, wall2TexBufID, TERRAIN_TEX_TAVERN_WALL_ID);
}
