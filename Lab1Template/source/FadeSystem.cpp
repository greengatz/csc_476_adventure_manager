/**
 * FadeSystem.cpp - Fade represents fading into another scene.
 * @author Brandon Clark
 */

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "FadeSystem.h"
#include "GLSL.h"
#include "shader.hpp"

FadeSystem::FadeSystem() :
   pid(0),
   h_vertPos(0),
   posBufObjOverlay(0),
   indBufObjOverlay(0),
   g_GiboLen(0),

   //attributes
   fadeStartTime(0),
   fadeDuration(2),
   currentlyFading(false),
   changeScene(false),
   hasChanged(false),

   //the resolution
   width(0),
   height(0),

   h_alpha(0)
{
}

FadeSystem::~FadeSystem()
{
}

bool FadeSystem::isFading()
{
   return currentlyFading;
}

bool FadeSystem::readyToChangeScene()
{
   return changeScene;
}

void FadeSystem::startFade(int aWidth, int aHeight)
{
   fadeStartTime = glfwGetTime();
   width = aWidth;
   height = aHeight;
   currentlyFading = true;
}

void FadeSystem::init()
{
   // Initialize Shader
   pid = LoadShaders( "Shaders/Fade_vert.glsl", "Shaders/Fade_frag.glsl");

   h_vertPos = GLSL::getAttribLocation(pid, "vertPos");
   h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");
   h_alpha = GLSL::getUniformLocation(pid, "alphaValue");

   GLfloat overlayPlane[] = {
      0, 0, 1.0f,
      0, 768.0f, 1.0f,
      1024.0f, 768.0f, 1.0f,
      1024.0f, 0, 1.0f 
   };

   glGenBuffers(1, &posBufObjOverlay);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjOverlay);
   glBufferData(GL_ARRAY_BUFFER, sizeof(overlayPlane), overlayPlane, GL_STATIC_DRAW);

   unsigned short idx[] = {0, 1, 2, 0, 2, 3};

   g_GiboLen = 6;
   glGenBuffers(1, &indBufObjOverlay);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjOverlay);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
   
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

void FadeSystem::updateFade()
{
   glDisable(GL_DEPTH_TEST);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClearColor(0.0, 0.0, 0.0, 0.0);

   //Using another shader program
   glUseProgram(pid);

   float dtTime = glfwGetTime() - fadeStartTime;

   if (dtTime < fadeDuration)
   {
      glUniform1f(h_alpha, (float)dtTime/fadeDuration);
      printf("ALPHA(OUT): %lf\n", dtTime/fadeDuration);
      drawFadeOut();
   }
   else if (dtTime < fadeDuration * 2.0)
   {
      glUniform1f(h_alpha, (1.0 - (dtTime - fadeDuration)/fadeDuration));
      printf("ALPHA(IN): %lf\n", (1.0 - (dtTime - fadeDuration)/fadeDuration));
      drawFadeOut();
      if (hasChanged)
      {
         changeScene = false;
      }
      else
      {
         changeScene = true;
         hasChanged = true;
      }
   }
   else
   {
      currentlyFading = false;
      changeScene = false;
      hasChanged = false;
   }
   printf("dtTime: %lf\n", dtTime);

   // Unbind the program
   glUseProgram(0);
   glEnable(GL_DEPTH_TEST);
}

//Private Function
void FadeSystem::drawFadeOut()
{
   GLSL::enableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjOverlay);
   glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjOverlay);

   glm::mat4 theOrtho = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
   glm::mat4 guiMVP;
   guiMVP = theOrtho * glm::mat4(1.0f); // Identity Matrix
   glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(guiMVP));
   
   glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0); // draw first object

   GLSL::disableVertexAttribArray(h_vertPos);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}