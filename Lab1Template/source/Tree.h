/**
 * Terrain.h - Header file for Terrain object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _TREE_H_
#define _TREE_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include <vector>
#include "Camera.h"
#include "RenderingHelper.h"
#include "Materials.h"

class Tree
{
public:
   Tree();
   virtual ~Tree();
   void init(TextureLoader* texLoader);
   void draw(glm::vec3 treePosition, Camera* camera, glm::vec3 wagonPos);
   void drawBillboard(glm::vec3 treePosition, Camera *camera, glm::vec3 wagonPos);

private:
   GLuint pid;
   GLint h_vertPos;
   GLint h_vertNor;
   GLint h_aTexCoord;

   //Handles to the shader data
   GLint h_uTexUnit;
   GLint h_ProjMatrix;
   GLint h_ViewMatrix;
   GLint h_ModelMatrix;

   //Material Color
   GLint h_ka;
   GLint h_kd;
   GLint h_ks;
   GLint h_s;

   //BARK Buffer
   GLuint posBufObjTree;
   GLuint norBufObjTree;
   GLuint treeTexBuffObj;
   GLuint indBufObjTree;

   //LEAFS buffer
   GLuint posBufObjLeaf;
   GLuint norBufObjLeaf;
   GLuint leafTexBuffObj;
   GLuint indBufObjLeaf;

   //Board
   GLuint posBufObjBoard;
   GLuint norBufObjBoard;
   GLuint boardTexBuffObj;

   GLuint leafToggleID;

   //Tree position.
   glm::vec3 position;
   //Light position
   GLuint lightPosID;

   //Rendering Helper
   RenderingHelper ModelTrans;
   std::vector<tinyobj::shape_t> shapes;
   float scale;

   Materials matSetter;
};

#endif