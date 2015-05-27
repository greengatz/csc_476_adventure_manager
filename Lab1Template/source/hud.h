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

using namespace std;
using namespace glm;

class HUD
{
	public:
		
		HUD(Manager *newMan);
		void drawHud(Camera * camera, int width, int height);
		void initHUD(TextureLoader *texLoader);
		void initHomeScreen(TextureLoader *texLoader);
		void enableBuff();
		void disableBuff();//GLint h_vertPos, GLint h_aTexCoord);
		GLuint posBufObjHUD;
		GLuint posBufObjMenu;

		GLuint texBuffObjMenu;
		GLuint indxBuffObjMenu;
		GLuint GIndxBuffObj;

		GLuint colorBufObjHUD;

		GLuint GrndTexBuffObj;
		GLuint textBuffMenu;

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
};

#endif
