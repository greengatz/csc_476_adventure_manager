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

Tree::Tree() :
   position(-75.0f, 0.0f, -25.0f),
   //Skybox Buffer
   posBufObjTree(0),
   norBufObjTree(0),
   treeTexBuffObj(0),
   indBufObjTree(0),

   posBufObjLeaf(0),
   norBufObjLeaf(0),
   leafTexBuffObj(0),
   indBufObjLeaf(0),

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
   
   GLSL::checkVersion();
   //printf("GL ERROR: %d\n", glGetError());
   assert(glGetError() == GL_NO_ERROR);
}

void Tree::draw(Camera *camera, glm::vec3 wagonPos)
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

   //Position Wagon along the trail
   ModelTrans.pushMatrix();
      ModelTrans.translate(position);
      ModelTrans.scale(scale, scale, scale);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
   ModelTrans.popMatrix();

   //set up the texture unit
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   //=============== BARK of TREE ===================/

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
   glUniform1i(leafToggleID, 1);

   //TODO: Set material when we go back to lighting methods. 

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