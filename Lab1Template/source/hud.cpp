#include "hud.h"

HUD::HUD(Manager *newMan)
{
	man = newMan;
	posBufObjHUD = 0;
	colorBufObjHUD = 0;
}

void HUD::initHUD()
{
	GLuint temp;
	// Initialize the VBO data
	float vert[9], colr[9]; // array variables to hold vertex data
	 
	// These values are according to screen size of 1920p width and 1080p height
	vert[0] = 960.0f; vert[1] = 540.0f; vert[2] = 1.0f; // TOP
	vert[3] = 900.0f; vert[4] = 10.0f; vert[5] = 1.0f; // LEFT
	vert[6] = 500.0f; vert[7] = 540.0f; vert[8] = 1.0f; // RIGHT

    glGenBuffers(1, &posBufObjHUD);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vert, GL_STATIC_DRAW);
	 
	colr[0] = 1.0f; colr[1] = 0.0f; colr[2] = 0.0f; // TOP
	colr[3] = 0.0f; colr[4] = 1.0f; colr[5] = 0.0f; // LEFT
	colr[6] = 0.0f; colr[7] = 0.0f; colr[8] = 1.0f; // RIGHT
	// COLOR VBO
	glGenBuffers(1, &colorBufObjHUD);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufObjHUD);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colr, GL_STATIC_DRAW);
}

void HUD::drawHud(GLint h_ModelMatrix, GLint h_vertPos, GLint h_hudColor, int width, int height)
{
	enableBuff(h_vertPos, h_hudColor);

	mat4 Ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
 
	glDisable(GL_DEPTH_TEST); // Disable the Depth-testing
	 
	glm::mat4 _guiMVP;
	_guiMVP = Ortho * glm::mat4(1.0f); // Identity Matrix
	 
	glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));
	 
	glDrawArrays(GL_TRIANGLES, 0, 3); // draw first object
	 
	glEnable(GL_DEPTH_TEST); // Enable the Depth-testing

	disableBuff(h_vertPos, h_hudColor);
}

void HUD::enableBuff(GLint h_vertPos, GLint h_hudColor) {
  GLSL::enableVertexAttribArray(h_vertPos); //position
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

  GLSL::enableVertexAttribArray(h_hudColor);
  glBindBuffer(GL_ARRAY_BUFFER, colorBufObjHUD);
  glVertexAttribPointer(h_hudColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void HUD::disableBuff(GLint h_vertPos, GLint h_hudColor) {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_hudColor);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}