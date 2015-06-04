/**
 * FadeSystem.h - Header file for FadeSystem.
 * @author Brandon Clark
 */

#pragma once
#ifndef _FADESYSTEM_H_
#define _FADESYSTEM_H_

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "TextureLoader.h"
#include <vector>

class FadeSystem
{
public:
   FadeSystem();
   bool dontToggleView;
   virtual ~FadeSystem();
   void init();

   bool isFading();
   bool readyToChangeScene();
   void startFade(int aWidth, int aHeight);
   void updateFade();

private:
   GLuint pid;
   GLint h_vertPos;

   //Handles to the shader data
   GLint h_ModelMatrix;
   int g_GiboLen;
   GLint h_alpha;

   GLuint posBufObjOverlay;
   GLuint indBufObjOverlay;

   float fadeStartTime;
   float fadeDuration;
   bool currentlyFading;
   bool changeScene;
   bool hasChanged;
   int width;
   int height;

   void drawFadeOut();
};

#endif