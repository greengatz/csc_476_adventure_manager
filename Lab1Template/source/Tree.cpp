/**
 * Skybox.cpp - Skybox for the terrain of wagon scene.
 * @author Brandon Clark
 */

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Tree.h"
#include "GLSL.h"
#include "shader.hpp"
#include "MatrixStack.h"

int TREE_BARK_TEX = 80;
int TREE_LEAFS_TEX = 81;
int TREE1_BOARD_TEX = 82;

Tree::Tree() :
   position(-100.0f, 0.0f, 25.0f),
   lightPosID(0),
   //Skybox Buffer
   posBufObjTree(0),
   norBufObjTree(0),
   treeTexBuffObj(0),
   indBufObjTree(0),

   posBufObjLeaf(0),
   norBufObjLeaf(0),
   leafTexBuffObj(0),
   indBufObjLeaf(0),

   posBufObjBoard(0),
   norBufObjBoard(0),
   boardTexBuffObj(0),

   //Initialize the material.
   h_ka(0),
   h_kd(0),
   h_ks(0),
   h_s(0),

   leafToggleID(0),

   scale(0.05f)
{
}

Tree::~Tree()
{
}

void Tree::init(TextureLoader* texLoader)
{
   //initialize the modeltrans matrix stack
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

   matSetter.init(&pid, &h_ka, &h_kd, &h_ks, &h_s);

  // Initialize Shader
  pid = LoadShaders( "Shaders/tree_vert.glsl", "Shaders/tree_frag.glsl");

  h_vertPos = GLSL::getAttribLocation(pid, "vertPos");
  h_vertNor = GLSL::getAttribLocation(pid, "vertNor");
  h_aTexCoord = GLSL::getAttribLocation(pid, "aTexCoord");
  h_ProjMatrix = GLSL::getUniformLocation(pid, "uProjMatrix");
  h_ViewMatrix = GLSL::getUniformLocation(pid, "uViewMatrix");
  h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");
  h_uTexUnit = GLSL::getUniformLocation(pid, "uTexUnit");
  leafToggleID = GLSL::getUniformLocation(pid, "leafToggle");
  lightPosID = GLSL::getUniformLocation(pid, "lightPos");
  h_ka = GLSL::getUniformLocation(pid, "ka");
  h_kd = GLSL::getUniformLocation(pid, "kd");
  h_ks = GLSL::getUniformLocation(pid, "ks");
  h_s = GLSL::getUniformLocation(pid, "s");

  // Load geometry
  // Some obj files contain material information.
  std::vector<tinyobj::material_t> objMaterials;
  std::string meshName = "assets/trees/tree1/tree1a_lod0.obj";
  string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str());
  if(!err.empty()) {
    cerr << err << endl;
  }

  //TREE BARK INIT

   // Send the position array to the GPU
   const vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufObjTree);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjTree);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

   //Set texture coordinates for the bark of the tree.
   const vector<float> &texBufTree = shapes[0].mesh.texcoords;
   glGenBuffers(1, &treeTexBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, treeTexBuffObj);
   glBufferData(GL_ARRAY_BUFFER, texBufTree.size()*sizeof(float), &texBufTree[0], GL_STATIC_DRAW);

   //Set the normals of the rock
   const vector<float> &norBuf = shapes[0].mesh.normals;
   glGenBuffers(1, &norBufObjTree);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjTree);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufObjTree);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjTree);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

   //unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   //===========================TREE LEAFS INIT======================================================

   // Send the position array to the GPU
   const vector<float> &posBufLeaf = shapes[1].mesh.positions;
   glGenBuffers(1, &posBufObjLeaf);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjLeaf);
   glBufferData(GL_ARRAY_BUFFER, posBufLeaf.size()*sizeof(float), &posBufLeaf[0], GL_STATIC_DRAW);

   //Set texture coordinates for the bark of the tree.
   const vector<float> &texBufLeaf = shapes[1].mesh.texcoords;
   glGenBuffers(1, &leafTexBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, leafTexBuffObj);
   glBufferData(GL_ARRAY_BUFFER, texBufLeaf.size()*sizeof(float), &texBufLeaf[0], GL_STATIC_DRAW);

   //Set the normals of the rock
   const vector<float> &norBufLeaf = shapes[1].mesh.normals;
   glGenBuffers(1, &norBufObjLeaf);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjLeaf);
   glBufferData(GL_ARRAY_BUFFER, norBufLeaf.size()*sizeof(float), &norBufLeaf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBufLeaf = shapes[1].mesh.indices;
   glGenBuffers(1, &indBufObjLeaf);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjLeaf);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBufLeaf.size()*sizeof(unsigned int), &indBufLeaf[0], GL_STATIC_DRAW);

   //Load Texture
   texLoader->LoadTexture((char *)"assets/trees/tree1/bark1.bmp", TREE_BARK_TEX);
   texLoader->LoadTexture((char *)"assets/trees/tree1/leafs1.bmp", TREE_LEAFS_TEX);

   //unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   //=================================Initialize Billboards=========================

   float PlanePos[] = {
    -1.0, -1.0, 0.0,
    -1.0,  1.0, 0.0,
     1.0, -1.0, 0.0,
     1.0,  1.0, 0.0
  };

  float normal_data[] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
  };

   static GLfloat PlaneTex[] = {      
      1.00, 0.00,
      1.00, 1.00,
      0.00, 0.00,
      0.00, 1.00
    }; 

    glGenBuffers(1, &posBufObjBoard);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjBoard);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlanePos), PlanePos, GL_STATIC_DRAW);

    glGenBuffers(1, &norBufObjBoard);
    glBindBuffer(GL_ARRAY_BUFFER, norBufObjBoard);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal_data), normal_data, GL_STATIC_DRAW);

    glGenBuffers(1, &boardTexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, boardTexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneTex), PlaneTex, GL_STATIC_DRAW);

     texLoader->LoadTexture(
      (char *)"assets/trees/billboard/tree1.bmp",
      TREE1_BOARD_TEX);

   //unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   
   GLSL::checkVersion();
   //printf("GL ERROR: %d\n", glGetError());
   assert(glGetError() == GL_NO_ERROR);
}

void Tree::drawBillboard(glm::vec3 treePosition, Camera *camera, glm::vec3 wagonPos)
{
   //Using another shader program
   glUseProgram(pid);

   glUniform1i(leafToggleID, 1);

   //Set projection matrix
   MatrixStack proj, view;
   proj.pushMatrix();
   camera->applyProjectionMatrix(&proj);
   glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
   proj.pushMatrix();
   camera->applyViewMatrix(&view, wagonPos);
   glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));

   glUniform3fv(lightPosID, 1, glm::value_ptr(glm::vec3(-75.0f, 0.0f, -25.0f)));

   glm::vec3 camPos = camera->getPosition();

   glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
   glm::vec3 look = camPos - 
      (treePosition + position + glm::vec3(0.0, 0.88, 0.0));
   look.y = 0.0;
   look = normalize(look);
   glm::vec3 lookAt = glm::vec3(0.0, 0.0, -1.0);

   float angle = glm::dot(lookAt, look);
   glm::vec3 upAux = glm::cross(lookAt, look);
   float rot = acos(angle)*180/3.14;

   //Position Wagon along the trail
   ModelTrans.pushMatrix();
      ModelTrans.translate(treePosition + position + glm::vec3(0.0, 0.8, 0.0));
      ModelTrans.rotate(rot, upAux);
      ModelTrans.scale(0.9, 0.9, 0.9);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
   ModelTrans.popMatrix();

   //set up the texture unit
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_vertNor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjBoard);
   glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjBoard);
   glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, boardTexBuffObj);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   //Draw Back plane
   glBindTexture(GL_TEXTURE_2D, TREE1_BOARD_TEX);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   GLSL::disableVertexAttribArray(h_vertPos);
   GLSL::disableVertexAttribArray(h_vertNor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisable(GL_TEXTURE_2D);

   glUniform1i(leafToggleID, 0);

   // Unbind the program
   glUseProgram(0);
   // Pop stacks
   proj.popMatrix();
}

void Tree::draw(glm::vec3 treePosition, Camera *camera, glm::vec3 wagonPos)
{
   //Using another shader program
   glUseProgram(pid);

   glUniform1i(leafToggleID, 0);

   //Set projection matrix
   MatrixStack proj, view;
   proj.pushMatrix();
   camera->applyProjectionMatrix(&proj);
   glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
   proj.pushMatrix();
   camera->applyViewMatrix(&view, wagonPos);
   glUniformMatrix4fv(h_ViewMatrix, 1, GL_FALSE, glm::value_ptr(view.topMatrix()));

   glUniform3fv(lightPosID, 1, glm::value_ptr(glm::vec3(-75.0f, 0.0f, -25.0f)));

   //Position Wagon along the trail
   ModelTrans.pushMatrix();
      ModelTrans.translate(treePosition + position); //this was just "position". Can get rid of this after.
      ModelTrans.scale(scale, scale, scale);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
   ModelTrans.popMatrix();

   //set up the texture unit
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   //=============== BARK of TREE ===================/

   //Set the material for the tree bark.
   matSetter.setMaterial(4);

   glBindTexture(GL_TEXTURE_2D, TREE_BARK_TEX);
   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, treeTexBuffObj);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjTree);
   glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_vertNor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjTree);
   glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Bind index array for drawing
   int nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjTree);

   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_vertPos);
   GLSL::disableVertexAttribArray(h_vertNor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   //======================== Draw Leaves of the tree ==========================

   //Set the material for the tree leaves.
   matSetter.setMaterial(1);

   glUniform1i(leafToggleID, 1);

   //set up the texture unit for bark
   glBindTexture(GL_TEXTURE_2D, TREE_LEAFS_TEX);
   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, leafTexBuffObj);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjLeaf);
   glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_vertNor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjLeaf);
   glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Bind index array for drawing
   nIndices = (int)shapes[1].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjLeaf);

   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_vertPos);
   GLSL::disableVertexAttribArray(h_vertNor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glUniform1i(leafToggleID, 0);

   // Unbind the program
   glUseProgram(0);
   // Pop stacks
   proj.popMatrix();
}