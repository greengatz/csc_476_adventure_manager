#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstdarg>
#include "GLSL.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "tavern.h"
#include "manager.h"
#include "TextureLoader.h"
#include "text2D.hpp"

using namespace std;
using namespace glm;

class Menu
{
	public:
		void drawMenu(int args, char* title, char* about, ...);
		void initMenu(TextureLoader *texLoader, GLint h_ModelMatrix, GLint h_vertPos, int width, int height, GLint h_aTexCoord);
		void enableBuff(GLint h_vertPos, GLint h_aTexCoord);
		void disableBuff(GLint h_vertPos, GLint h_aTexCoord);
		GLuint posBufObjHUD;
		GLuint GrndTexBuffObj;
		GLuint GIndxBuffObj;
		int on;

	private:
		int g_GiboLen;
		GLint h_ModelMatrix;
		GLint h_vertPos;
		int width;
		int height;
		GLint h_aTexCoord;
		int offset = -40;
};

#endif