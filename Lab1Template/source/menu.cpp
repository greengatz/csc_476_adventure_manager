#include "menu.h"

int MENU_ID = 4005;

void Menu::initMenu(Camera * cameraA, TextureLoader *texLoader, int widthA, int heightA, Manager *mgr, bool* gameP)
{
	ModelTrans.useModelViewMatrix();
	ModelTrans.loadIdentity();

	// Initialize Shader
	pid = LoadShaders( "Shaders/HUD_vert.glsl", 
	    "Shaders/HUD_frag.glsl" );

	h_vertPos = GLSL::getAttribLocation(pid, "vertPos");
	h_vertNor = GLSL::getAttribLocation(pid, "vertNor");
	h_aTexCoord = GLSL::getAttribLocation(pid, "aTexCoord");
	h_ProjMatrix = GLSL::getUniformLocation(pid, "uProjMatrix");
	h_ViewMatrix = GLSL::getUniformLocation(pid, "uViewMatrix");
	h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");
	h_uTexUnit = GLSL::getUniformLocation(pid, "uTexUnit");

	camera = cameraA;
	gamePaused = gameP;
	manager = mgr;
	inMenu = false;
	width = widthA;
	height = heightA;
	posBufObjHUD = 0;
	GrndTexBuffObj = 0;
	GIndxBuffObj = 0;
	initText2D( "Holstein.DDS" );
	texLoader->LoadTexture((char *)"assets/newMenu.bmp", MENU_ID);

	//End points of Menu
	GLfloat vert[] = {
		128.0f, 128.0f, 1.0f,
		128.0f, 640.0f, 1.0f,
		896.0f, 640.0f, 1.0f,
		896.0f, 128.0f, 1.0f
	};

    glGenBuffers(1, &posBufObjHUD);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	//Maps UVs
	// static GLfloat GrndTex[] = {
 //      0, 1, // back
 //      0, 0,
 //      1, 0,
 //      1, 1 };

	static GLfloat GrndTex[] = {
      0, 0,
      0, 1, // back
      1, 1, 
      1, 0};

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    g_GiboLen = 6;
    glGenBuffers(1, &GrndTexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &GIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}


void Menu::setData(char* titleA, vector<string> aboutA, vector<option> optionsA, tdogl::pngTexture** taet, int type, vector<string> newData)
{

	inMenu = true;
	title = titleA;
	about = aboutA;
	options = optionsA;
	myTexture = *taet;
	menuType = type;
	data = newData;
	printf("menu true\n");
}

void Menu::selectOption(int num)
{
	if(num < options.size())
	{
		if(options[num].close == true){
			inMenu = false;
		}
		options[num].funct(manager, gamePaused);
	}
}

void Menu::drawMenu()
{
	glUseProgram(pid);
	MatrixStack proj;
	proj.pushMatrix();
	camera->applyProjectionMatrix(&proj);
	glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	proj.pushMatrix();

	//Enable Buffers
	glUseProgram(pid);
	enableBuff();

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
	disableBuff();

	if(menuType == 0)
	{
		//nothing
	}
	else if(menuType == 1)
	{

		//merchant menu - meat beer
		printText2D(data[1].c_str(), 280, 304, 15);
		printText2D(data[0].c_str(), 280, 325, 15);
	}
	else if(menuType == 2)
	{
		// sickness
		printText2D(data[2].c_str(), 248, 294, 16);
		printText2D(data[3].c_str(), 357, 294, 16);
		// printText2D(data[2].c_str(), 210, 204, 15);
		// good
	}
	else if(menuType == 3)
	{
		// buy merc
		// gold
		// printText2D(data[0].c_str(), 205, 345, 16);
		printText2D(data[1].c_str(), 407, 297, 16);
		// good
	}
	else if(menuType == 4)
	{
		//bandits
		//nothing
	}
	else if(menuType == 5)
	{
		//beggar
		//gold
		printText2D(data[0].c_str(), 269, 344, 16);
		// printText2D(data[0].c_str(), 200, 200, 20);
		// good
	}
	else if(menuType == 6)
	{
		//oops
		//nothing
	}
	else if(menuType == 7)
	{
		// ?
	}
	else if(menuType == 8)
	{
		// merchant robbed
		printText2D(data[0].c_str(), 301, 348, 16);
		// good
	}
	else if(menuType == 9)
	{
		// fight won gold beer food
		printText2D(data[0].c_str(), 299, 329, 16);
		printText2D(data[1].c_str(), 383, 329, 16);
		printText2D(data[2].c_str(), 496, 329, 16);
		// good
	}
	else if(menuType == 10)
	{
		// die
		// nothing
	}
	else if(menuType == 11)
	{
		// coward flee gold beer food
		printText2D(data[0].c_str(), 294, 321, 16); // gold
		// printText2D(data[3].c_str(), 357, 294, 16);

		//add food and beer here
	}
	else if(menuType == 12)
	{
		// skilled merchant
		// nothing
		// good
	}
	else if(menuType == 13)
	{
		// cant heal
	}
	else if(menuType == 14)
	{
		// noting max mercs
		// good
	}
	else if(menuType == 15)
	{
		// no gold wanderer
		// nothing
		// good
	}

	//sprintf(info,"x %d", manager.getMercs());
	//printText2D(title, 350, 470, 28);
	// printText2D(about, 160, 440, 14);
	// for(int i = 0; i < about.size(); i++)
	// {
	// 	printText2D(about[i].c_str(), 160, 440 + aboutOffset, 14);
	// 	aboutOffset -= 20;
	// }
	// aboutOffset = -20;

	// for(int i = 0; i < options.size(); i++)
	// {
	// 	printText2D(options[i].str.c_str(), 160, 400 + offset, 18);
	// 	offset-=40;
	// }

	// offset = -40;
}

void Menu::enableBuff() {
	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

  GLSL::enableVertexAttribArray(h_vertPos); //position
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjHUD);
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindTexture(GL_TEXTURE_2D, myTexture->object());

  GLSL::enableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
  glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT ,GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
}

void Menu::disableBuff() {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisable(GL_TEXTURE_2D);
}

void print0()
{
	
}

void print1()
{

}

void print3()
{
	
}

void print4()
{

}