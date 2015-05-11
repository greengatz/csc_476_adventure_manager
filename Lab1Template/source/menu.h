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

struct option
{
	string str;
	void (* funct)(void *, bool *);
	bool close;
};

class Menu
{
	public:
		void drawMenu();
		void initMenu(TextureLoader *texLoader, GLint h_ModelMatrix, GLint h_vertPos, int width, int height, GLint h_aTexCoord, Manager *mgr, bool* gameP);
		void enableBuff(GLint h_vertPos, GLint h_aTexCoord);
		void disableBuff(GLint h_vertPos, GLint h_aTexCoord);
		void setData(char* title, vector<string> about, vector<option> options);
		void selectOption(int num);
		GLuint posBufObjHUD;
		GLuint GrndTexBuffObj;
		GLuint GIndxBuffObj;
		bool inMenu;


	private:
		char* title;
		vector<string> about;
		vector<option> options;
		int g_GiboLen;
		GLint h_ModelMatrix;
		GLint h_vertPos;
		int width;
		int height;
		GLint h_aTexCoord;
		int offset = -40;
		int aboutOffset = -20;
		Manager *manager;
		bool* gamePaused;
};

#endif