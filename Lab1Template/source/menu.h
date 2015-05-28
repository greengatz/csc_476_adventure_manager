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
#include "shader.hpp"
#include "MatrixStack.h"
#include "RenderingHelper.h"

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
		void initMenu(Camera * camera, TextureLoader *texLoader, int width, int height, Manager *mgr, bool* gameP);
		void enableBuff();
		void disableBuff();
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
		int width;
		int height;
		int offset = -40;
		int aboutOffset = -20;
		Manager *manager;
		bool* gamePaused;
		Camera * camera;

		GLuint pid;
		GLint h_vertPos;
		GLint h_vertNor;
		GLint h_aTexCoord;
		//Handles to the shader data
		GLint h_uTexUnit;
		GLint h_ProjMatrix;
		GLint h_ViewMatrix;
		GLint h_ModelMatrix;

		RenderingHelper ModelTrans;
};

#endif