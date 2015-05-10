/**
 * Skybox.cpp - Skybox for the terrain of wagon scene.
 * @author Brandon Clark
 */

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Skybox.h"
#include "GLSL.h"
#include "shader.hpp"
#include "MatrixStack.h"

int SKYBOX_FRONT = 75;
int SKYBOX_LEFT = 76;
int SKYBOX_BACK = 77;
int SKYBOX_RIGHT = 78;
int SKYBOX_TOP = 79;

Skybox::Skybox() :
	position(-75.0f, 0.0f, -25.0f),
   //Skybox Buffer
   posBufObjSkybox(0),
   norBufObjSkybox(0),
   skyTexBuffObj(0)
{
}

Skybox::~Skybox()
{
}

void Skybox::init(TextureLoader* texLoader)
{
   //initialize the modeltrans matrix stack
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

  // Initialize Shader
  pid = LoadShaders( "Shaders/skybox_vert.glsl", 
      "Shaders/skybox_frag.glsl" );

  h_vertPos = GLSL::getAttribLocation(pid, "vertPos");
  h_vertNor = GLSL::getAttribLocation(pid, "vertNor");
  h_aTexCoord = GLSL::getAttribLocation(pid, "aTexCoord");
  h_ProjMatrix = GLSL::getUniformLocation(pid, "uProjMatrix");
  h_ViewMatrix = GLSL::getUniformLocation(pid, "uViewMatrix");
  h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");
  h_uTexUnit = GLSL::getUniformLocation(pid, "uTexUnit");

  // Check GLSL
  GLSL::checkVersion();
  assert(glGetError() == GL_NO_ERROR);

  float CubePos[] = {
    -1.0, -1.0, -1.0, //back face 5 verts :0 - clockwise 
    -1.0,  1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,

     1.0, -1.0,  1.0, //right face 5 verts :4
     1.0,  1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,

    -1.0, -1.0,  1.0, //front face 4 verts :8
    -1.0,  1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,

    -1.0, -1.0, -1.0, //left face 4 verts :12
    -1.0,  1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,

    -1.0,  1.0,  -1.0, //top face of cube :16
     -1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0, 1.0
  };

  float normal_data[] = {
    0.0, 0.0, 1.0, //back normals
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    -1.0, 0.0, 0.0, //right normals
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,

    0.0, 0.0, -1.0, //front normals
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,

    1.0, 0.0, 0.0, //left normals
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,

    0.0, -1.0, 0.0, //top normals
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0
  };

   static GLfloat CubeTex[] = {
      
      1.00, 0.00,
      1.00, 1.00, //back
      0.00, 0.00,
      0.00, 1.00,
      
      0.00, 0.00, //right
      0.00, 1.00, 
      1.00, 0.00,
      1.00, 1.00,

      0.00, 0.00, //front
      0.00, 1.00, 
      1.00, 0.00,
      1.00, 1.00,

      0.00, 0.00, //left
      0.00, 1.00, 
      1.00, 0.00,
      1.00, 1.00,

      0.00, 1.00, //top
      0.00, 0.00, 
      1.00, 1.00,
      1.00, 0.00
    }; 

    glGenBuffers(1, &posBufObjSkybox);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjSkybox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubePos), CubePos, GL_STATIC_DRAW);

    glGenBuffers(1, &norBufObjSkybox);
    glBindBuffer(GL_ARRAY_BUFFER, norBufObjSkybox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal_data), normal_data, GL_STATIC_DRAW);

    glGenBuffers(1, &skyTexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, skyTexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTex), CubeTex, GL_STATIC_DRAW);

     texLoader->LoadTexture(
      (char *)"assets/skyboxes/powderpeak/powderpeak_front.bmp", 
      SKYBOX_FRONT);

     texLoader->LoadTexture(
      (char *)"assets/skyboxes/powderpeak/powderpeak_right.bmp", 
      SKYBOX_RIGHT);

     texLoader->LoadTexture(
      (char *)"assets/skyboxes/powderpeak/powderpeak_back.bmp", 
      SKYBOX_BACK);

     texLoader->LoadTexture(
      (char *)"assets/skyboxes/powderpeak/powderpeak_left.bmp", 
      SKYBOX_LEFT);

     texLoader->LoadTexture(
      (char *)"assets/skyboxes/powderpeak/powderpeak_top.bmp", 
      SKYBOX_TOP);

   //unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::draw(Camera *camera, glm::vec3 wagonPos)
{
   //Using another shader program
   glUseProgram(pid);

   //Set projection matrix
   MatrixStack proj, view;
   proj.pushMatrix();
   camera->applyProjectionMatrix(&proj);
   glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
   proj.pushMatrix();
   camera->applyViewMatrix(&view, wagonPos);
   glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));

   //Position Wagon along the trail
   ModelTrans.pushMatrix();
      ModelTrans.translate(position);
      ModelTrans.scale(PLANE_SCALE, PLANE_SCALE*0.75, PLANE_SCALE);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
   ModelTrans.popMatrix();


   //set up the texture unit
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_vertNor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjSkybox);
   glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjSkybox);
   glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, skyTexBuffObj);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   //Draw Back plane
   glBindTexture(GL_TEXTURE_2D, SKYBOX_BACK);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   //Draw right plane
   glBindTexture(GL_TEXTURE_2D, SKYBOX_RIGHT);
   glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

   //Draw front plane
   glBindTexture(GL_TEXTURE_2D, SKYBOX_FRONT);
   glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

   //Draw left plane
   glBindTexture(GL_TEXTURE_2D, SKYBOX_LEFT);
   glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

   //Draw top plane
   glBindTexture(GL_TEXTURE_2D, SKYBOX_TOP);
   glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

   GLSL::disableVertexAttribArray(h_vertPos);
   GLSL::disableVertexAttribArray(h_vertNor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisable(GL_TEXTURE_2D);

   // Unbind the program
   glUseProgram(0);
   // Pop stacks
   proj.popMatrix();
}