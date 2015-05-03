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

using namespace std;
using namespace glm;

class HUD
{
	public:
		
		HUD(Manager *newMan);
		void drawHud(GLint h_ModelMatrix, GLint h_vertPos, int width, int height, GLint h_aTexCoord);
		void initHUD(TextureLoader *texLoader);
		void enableBuff(GLint h_vertPos, GLint h_aTexCoord);
		void disableBuff(GLint h_vertPos, GLint h_aTexCoord);
		GLuint posBufObjHUD;
		GLuint colorBufObjHUD;
		GLuint GrndTexBuffObj;
		GLuint GIndxBuffObj;
		int on = 1;

	private:
		Manager *man;
};

#endif