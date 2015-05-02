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

using namespace std;
using namespace glm;

class HUD
{
	public:
		
		HUD(Manager *newMan);
		void drawHud(GLint h_ModelMatrix, GLint h_vertPos, GLint h_hudColor, int width, int height);
		void initHUD();
		void enableBuff(GLint h_vertPos, GLint h_hudColor);
		void disableBuff(GLint h_vertPos, GLint h_hudColor);
		GLuint posBufObjHUD;
		GLuint colorBufObjHUD;

	private:
		Manager *man;
};

#endif
