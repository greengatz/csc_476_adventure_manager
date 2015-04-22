/**
 * Wagon.cpp - Wagon along a trail.
 * @author Brandon Clark
 */

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Wagon.h"
#include "GLSL.h"
#include <math.h>

using namespace std;

int WAGON_TEX_ID = 111;

Wagon::Wagon() :
	position(0.6f, 0.05f, -0.5f),
	scale(0.03f, 0.03f, 0.03f),
  direction(0.0f, 0.0f, 0.0f),
  rotate(90.0f),
	posBufID(0),
	norBufID(0),
  indBufID(0),
	texBufID(0),
   startTime(0.0f),
   wagonStart(false),
   terrain(0),
   deltaTime(0.0f),
   velocity(0.00001f),
   nextPoint(0.0f, 0.0f, 0.0f)
{
}

Wagon::~Wagon()
{
}

void Wagon::resetWagon()
{
   wagonStart = false;
   position = terrain->getStartPosition() + glm::vec3(0.6, 0.05, -0.5);
   nextPoint = terrain->nextCriticalPoint(position);
   direction = glm::normalize(nextPoint - position);
   printf("Starting Position: <%lf, %lf, %lf>\n", position.x, position.y, position.z);
   terrain->printCriticalPoints();
}

void Wagon::startWagon()
{
   startTime = glfwGetTime();
   wagonStart = true;
}

void Wagon::updateWagon(float globalTime)
{
  if (wagonStart && !terrain->atEnd(position))
  {
    terrain->checkEvents(position);
    deltaTime = globalTime - startTime;
    if (position.x >= nextPoint.x)
    {

      nextPoint = terrain->nextCriticalPoint(position);
      direction = glm::normalize(nextPoint - position);
    }
    //printf("Position Before: <%lf, %lf, %lf>\n", position.x, position.y, position.z);
    position += direction * deltaTime * velocity;
    //printf("Position After: <%lf, %lf, %lf>\n", position.x, position.y, position.z);
  }
}

void Wagon::setPosition(float x, float y, float z)
{
   position.x = x;
   position.y = y;
   position.z = z;
}

glm::vec3 Wagon::getPosition()
{
   return glm::vec3(position.x - 100, position.y, position.z);
}

void Wagon::setScale(glm::vec3 aScale)
{
   scale = aScale;
}

void Wagon::setRotation(float aRotation)
{
   rotate = aRotation;
}

void Wagon::init(TextureLoader* texLoader, Terrain* aTerrain)
{
   terrain = aTerrain;
   //aPos relative to terrain generation
   resetWagon();

   // Load geometry
  // Some obj files contain material information.
  // We'll ignore them for this assignment.
  std::vector<tinyobj::material_t> objMaterials;
  std::string meshName = "assets/caravan/caravan.obj";
  string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str());
  if(!err.empty()) {
    cerr << err << endl;
  }

   // Send the position array to the GPU
   const vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufID);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

   //Set texture coordinates for the bark of the tree.
   const vector<float> &texBufRock = shapes[0].mesh.texcoords;
   glGenBuffers(1, &texBufID);
   glBindBuffer(GL_ARRAY_BUFFER, texBufID);
   glBufferData(GL_ARRAY_BUFFER, texBufRock.size()*sizeof(float), &texBufRock[0], GL_STATIC_DRAW);

   //Set the normals of the rock
   const vector<float> &norBuf = shapes[0].mesh.normals;
   glGenBuffers(1, &norBufID);
   glBindBuffer(GL_ARRAY_BUFFER, norBufID);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);


    //Load Texture
    texLoader->LoadTexture((char *)"assets/caravan/caravan_texture.bmp", WAGON_TEX_ID);

  	//unbind the arrays
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
   GLSL::checkVersion();
	 assert(glGetError() == GL_NO_ERROR);
}

void Wagon::draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, RenderingHelper *modelTrans)
{
   //Position Wagon along the trail
   modelTrans->pushMatrix();
      modelTrans->translate(position);
      modelTrans->rotate(rotate, glm::vec3(0, 1, 0));
      modelTrans->scale(scale.x, scale.y, scale.z);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(modelTrans->modelViewMatrix));
   modelTrans->popMatrix();

  //set up the texture unit
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, WAGON_TEX_ID);

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

   // Bind index array for drawing
   int nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_pos);
   GLSL::disableVertexAttribArray(h_nor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisable(GL_TEXTURE_2D);
}
