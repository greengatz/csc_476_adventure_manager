#ifndef HUD_H
#define HUD_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "GLSL.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "tavern.h"
#include "manager.h"
#include "TextureLoader.h"
#include "RenderingHelper.h"
#include "MatrixStack.h"
#include "shader.hpp"
#include "text2D.hpp"
#include "mercenary.h"

using namespace std;
using namespace glm;

class HUD
{
	public:
		
		HUD(Manager *newMan);
		void drawHud(Camera * camera, int width, int height);
		void drawSideHud(Camera * camera, int width, int height);
		void initHUD(TextureLoader *texLoader);
		void initHomeScreen(TextureLoader *texLoader);
		void initSideHud(TextureLoader *texLoader);
		void enableBuff(GLuint posBufObjA, int ID);
		void disableBuff();//GLint h_vertPos, GLint h_aTexCoord);
		GLuint posBufObjHUD;
		GLuint posBufObjSideHUD;
		GLuint posBufObjMenu;

		GLuint texBuffObjMenu;
		GLuint indxBuffObjMenu;
		GLuint indxBuffObjSideHUD;
		GLuint GIndxBuffObj;

		GLuint colorBufObjHUD;

		GLuint GrndTexBuffObj;
		GLuint textBuffMenu;
		GLuint textBuffSideHud;

		GLuint posBufObjFace;
		GLuint posBufObjHeart;
		GLuint posBufObjDam;

		bool on;
		bool homeScreenOn;
		bool deadScreenOn;

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
		Manager *man;
		int g_GiboLen;
		RenderingHelper ModelTrans;
		float offset = 0;
		int stringOffset = 0;
};

#endif
