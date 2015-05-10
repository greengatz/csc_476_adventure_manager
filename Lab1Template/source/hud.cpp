#include "hud.h"
int HUD_ID = 4000;
int HOME_ID = 4001;


HUD::HUD(Manager *newMan)
{
	man = newMan;
	posBufObjHUD = 0;
	colorBufObjHUD = 0;
	GrndTexBuffObj = 0;
	GIndxBuffObj = 0;
	posBufObjMenu = 0;
	indxBuffObjMenu = 0;
	textBuffMenu = 0;
	on = true;
	homeScreenOn = true;
}

void HUD::initHUD(TextureLoader *texLoader)
{
	texLoader->LoadTexture((char *)"assets/hud.bmp", HUD_ID);

	GLfloat vert[] = {
		0, 0, 1.0f,
		0, 64.0f, 1.0f,
		1024.0f, 64.0f, 1.0f,
		1024.0f, 0, 1.0f
	};

	GLfloat homeVerts[] = {
		0, 0, 1.0f,
		0, 768.0f, 1.0f,
		1024.0f, 768.0f, 1.0f,
		1024.0f, 0, 1.0f 
	};

    glGenBuffers(1, &posBufObjHUD);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	GLfloat colr[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
	};
	glGenBuffers(1, &colorBufObjHUD);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufObjHUD);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colr), colr, GL_STATIC_DRAW);

	static GLfloat GrndTex[] = {
      0, 1, // back
      0, 0,
      1, 0,
      1, 1 };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    g_GiboLen = 6;
    glGenBuffers(1, &GrndTexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &GIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

void HUD::initHomeScreen(TextureLoader *texLoader)
{
	texLoader->LoadTexture((char *)"assets/homeScreen.bmp", HOME_ID);


	GLfloat homeVerts[] = {
		0, 0, 1.0f,
		0, 768.0f, 1.0f,
		1024.0f, 768.0f, 1.0f,
		1024.0f, 0, 1.0f 
	};

    glGenBuffers(1, &posBufObjMenu);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjMenu);
	glBufferData(GL_ARRAY_BUFFER, sizeof(homeVerts), homeVerts, GL_STATIC_DRAW);

	static GLfloat GrndTex[] = {
      0, 1, // back
      0, 0,
      1, 0,
      1, 1 };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    g_GiboLen = 6;
    glGenBuffers(1, &textBuffMenu);
    glBindBuffer(GL_ARRAY_BUFFER, textBuffMenu);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &indxBuffObjMenu);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxBuffObjMenu);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

void HUD::drawHud(GLint h_ModelMatrix, GLint h_vertPos, int width, int height, GLint h_aTexCoord)
{
	enableBuff(h_vertPos, h_aTexCoord);

	mat4 Ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
 
	glDisable(GL_DEPTH_TEST); // Disable the Depth-testing
	 
	glm::mat4 _guiMVP;
	_guiMVP = Ortho * glm::mat4(1.0f); // Identity Matrix
	 
	glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
	
	glEnable(GL_DEPTH_TEST); // Enable the Depth-testing
	disableBuff(h_vertPos, h_aTexCoord);
}

void HUD::enableBuff(GLint h_vertPos, GLint h_aTexCoord) {
	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

  GLSL::enableVertexAttribArray(h_vertPos); //position
  if(homeScreenOn)
  {
  	glBindBuffer(GL_ARRAY_BUFFER, posBufObjMenu);
  }
  else
  {
  	glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
  }
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  if(homeScreenOn)
  {
  	glBindTexture(GL_TEXTURE_2D, HOME_ID);
  }
  else
  {
  	glBindTexture(GL_TEXTURE_2D, HUD_ID);
  }

  GLSL::enableVertexAttribArray(h_aTexCoord);

  if(homeScreenOn)
  {
  	glBindBuffer(GL_ARRAY_BUFFER, textBuffMenu);
  }
  else
  {
  	glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
  }
  glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT ,GL_FALSE, 0, 0);

  if(homeScreenOn)
  {
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxBuffObjMenu);
  }
  else
  {
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
  }
}

void HUD::disableBuff(GLint h_vertPos, GLint h_aTexCoord) {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisable(GL_TEXTURE_2D);
}