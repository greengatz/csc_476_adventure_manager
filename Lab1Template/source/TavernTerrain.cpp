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

void TavernTerrain::calcBuffers(int coor1, int coor2, GLuint *posBuf, GLuint *norBuf, GLuint *texBuf, 
                                float data[MAP_X][MAP_Z][3])
{
    GLfloat buffer[(MAP_Z - 1) * (MAP_X - 1) * 12];
    GLfloat norm[(MAP_Z - 1) * (MAP_X - 1) * 12];
    GLfloat tex[(MAP_Z - 1) * (MAP_X - 1) * 8];

    int index = 0;

    //initialize normals to zero
    for (int iter = 0; iter < (coor2 - 1) * (coor1 - 1) * 12; iter++)
    {
      buffer[iter] = 0;
      norm[iter] = 0;
    }

    index = 0;
    for (int iter = 0; iter < coor2 - 1; iter++)
    { 
      for (int iter2 = 0; iter2 < coor1 - 1; iter2++, index++)
      {

        //Calculate normal
        glm::vec3 v1 = glm::vec3(data[iter2][iter][0], data[iter2][iter][1], data[iter2][iter][2]);
        glm::vec3 v2 = glm::vec3(data[iter2+1][iter][0], data[iter2+1][iter][1], data[iter2+1][iter][2]);
        glm::vec3 v3 = glm::vec3(data[iter2][iter+1][0], data[iter2][iter+1][1], data[iter2][iter+1][2]);
        glm::vec3 nor = glm::cross(v2-v1, v3-v1);
      
        //v1
        buffer[12 * index + 0] = data[iter2][iter][0]; //x
        buffer[12 * index + 1] = data[iter2][iter][1]; //y
        buffer[12 * index + 2] = data[iter2][iter][2]; //z
        //add normal to sum
        norm[12 * index + 0] += nor.x;
        norm[12 * index + 1] += nor.y;
        norm[12 * index + 2] += nor.z;
        //set texture coordinate for that point
        tex[8 * index + 0] = 0.0;
        tex[8 * index + 1] = 0.0;

        //v2
        buffer[12 * index + 3] = data[iter2+1][iter][0];
        buffer[12 * index + 4] = data[iter2+1][iter][1];
        buffer[12 * index + 5] = data[iter2+1][iter][2];
        norm[12 * index + 3] += nor.x;
        norm[12 * index + 4] += nor.y;
        norm[12 * index + 5] += nor.z;
        tex[8 * index + 2] = 1.0;
        tex[8 * index + 3] = 0.0;

        //v3
        buffer[12 * index + 6] = data[iter2][iter+1][0];
        buffer[12 * index + 7] = data[iter2][iter+1][1];
        buffer[12 * index + 8] = data[iter2][iter+1][2];
        norm[12 * index + 6] += nor.x;
        norm[12 * index + 7] += nor.y;
        norm[12 * index + 8] += nor.z;
        tex[8 * index + 4] = 0.0;
        tex[8 * index + 5] = 1.0;

        //v4
        buffer[12 * index + 9] = data[iter2+1][iter+1][0];
        buffer[12 * index + 10] = data[iter2+1][iter+1][1];
        buffer[12 * index + 11] = data[iter2+1][iter+1][2];
        norm[12 * index + 9] += nor.x;
        norm[12 * index + 10] += nor.y;
        norm[12 * index + 11] += nor.z;
        tex[8 * index + 6] = 1.0;
        tex[8 * index + 7] = 1.0;
      }
    }

    //normalize the normals
    for (int iter = 0; iter < ((coor2 - 1) * (coor1 - 1) * 12)/3; iter += 3)
    {
      glm::vec3 nor = glm::normalize(glm::vec3(norm[iter], norm[iter+1], norm[iter+2]));

      norm[iter] = nor.x;
      norm[iter+1] = nor.y;
      norm[iter+2] = nor.z;
    }

    glGenBuffers(1, &(*posBuf));
    glBindBuffer(GL_ARRAY_BUFFER, *posBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
  
    glGenBuffers(1, &(*norBuf));
    glBindBuffer(GL_ARRAY_BUFFER, *norBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(norm), norm, GL_STATIC_DRAW);
  
    glGenBuffers(1, &(*texBuf));
    glBindBuffer(GL_ARRAY_BUFFER, *texBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
}

void TavernTerrain::init(TextureLoader* texLoader)
{
  ground = glm::vec3(5.0f, 0.0f, -5.0f);
  wall1 = glm::vec3(5.0f, 20.0f, -35.0f);
  wall2 = glm::vec3(7.5f, 20.0f, -35.0f);
	scale = 0.75f;

    // loop through all of the heightfield points, calculating
    // the coordinates for each point
    int index = 0;
    for (int z = 0; z < MAP_Z; z++)
    {
      for (int x = 0; x < MAP_X; x++, index++)
      {
        terrain[x][z][0] = float(x) * MAP_SCALE;    
        terrain[x][z][1] = 0; //restrict y to 0
        terrain[x][z][2] = -float(z) * MAP_SCALE;
      }
    }
    calcBuffers(MAP_X, MAP_Z, &posBufID, &norBufID, &texBufID, terrain);
    index = 0;
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
    calcBuffers(MAP_X, MAP_Y, &wallPosBufID, &wallNorBufID, &wallTexBufID, wallData);

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
    calcBuffers(MAP_Y, MAP_Z, &wall2PosBufID, &wall2NorBufID, &wall2TexBufID, wallData2);

    //Load Texture
    texLoader->LoadTexture((char *)"assets/tavern/stoneFloorTex.bmp", TERRAIN_TEX_TAVERN_FLOOR_ID);
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

   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, targetTex);

   //mipmap creation
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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
