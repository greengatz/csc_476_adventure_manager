#include "menu.h"

int MENU_ID = 4004;

void Menu::initMenu(TextureLoader *texLoader, GLint h_ModelMatrixA, GLint h_vertPosA, int widthA, int heightA, GLint h_aTexCoordA)
{
	h_ModelMatrix = h_ModelMatrixA;
	h_vertPos = h_vertPosA;
	width = widthA;
	height = heightA;
	h_aTexCoord = h_aTexCoordA;
	posBufObjHUD = 0;
	GrndTexBuffObj = 0;
	GIndxBuffObj = 0;
	on = true;
	initText2D( "Holstein.DDS" );
	texLoader->LoadTexture((char *)"assets/menuBack.bmp", MENU_ID);

	//End points of Menu
	GLfloat vert[] = {
		128.0f, 128.0f, 1.0f,
		128.0f, 640.0f, 1.0f,
		896.0f, 640.0f, 1.0f,
		896.0f, 128.0f, 1.0f
	};

	// GLfloat vert[] = {
	// 	128.0f, 0.0f, 1.0f,
	// 	128.0f, 512.0f, 1.0f,
	// 	640.0f, 512.0f, 1.0f,
	// 	640.0f, 0.0f, 1.0f
	// };
	// GLfloat vert[] = {
	// 	0, 0, 1.0f,
	// 	0, 64.0f, 1.0f,
	// 	1024.0f, 64.0f, 1.0f,
	// 	1024.0f, 0, 1.0f
	// };

    glGenBuffers(1, &posBufObjHUD);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	//Maps UVs
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

// void print() {
//     cout << endl;
// }

// template <typename T> void print(const T& t) {
//     cout << t;
// }

// template <typename First, typename... Rest> void print(const First& first, const Rest&... rest) {
//     print(first);
//     print(rest...); // recursive call using pack expansion syntax
// }

void Menu::drawMenu(int args, char* title, char* about, ...)
{
	va_list ap;
	va_start(ap, args);
	//va_end(ap);
	//Enable Buffers
	enableBuff(h_vertPos, h_aTexCoord);

	//Ortho Call
	mat4 Ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
 
	glDisable(GL_DEPTH_TEST); // Disable the Depth-testing
	 
	//Only pass Ortho Projection
	glm::mat4 _guiMVP;
	_guiMVP = Ortho * glm::mat4(1.0f); // Identity Matrix
	 
	glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
	
	glEnable(GL_DEPTH_TEST); // Enable the Depth-testing
	//Disable Buffers
	disableBuff(h_vertPos, h_aTexCoord);

	//sprintf(info,"x %d", manager.getMercs());
	printText2D(title, 350, 470, 28);
	printText2D(about, 160, 440, 14);

	for(int i = 0; i < args; i++)
	{
		char* sentence = va_arg(ap, char*);
		printText2D(sentence, 350, 400 + offset, 18);
		offset-=40;
	}
	va_end(ap);

	offset = -40;
}

void Menu::enableBuff(GLint h_vertPos, GLint h_aTexCoord) {
	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

  GLSL::enableVertexAttribArray(h_vertPos); //position
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindTexture(GL_TEXTURE_2D, MENU_ID);

  GLSL::enableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
  glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT ,GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
}

void Menu::disableBuff(GLint h_vertPos, GLint h_aTexCoord) {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisable(GL_TEXTURE_2D);
}

