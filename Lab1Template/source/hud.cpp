#include "hud.h"
int HUD_ID = 4000;
int SIDEHUD_ID = 4004;
int HOME_ID = 4001;
int DEAD_ID = 4002;
int MERCFACE_ID = 4003;
int DEADFACE_ID = 4006;
int SICKFACE_ID = 4010;
int HAPPYFACE_ID = 4011;
int SADFACE_ID = 4012;
int HEALTH_ID = 4013;
int DAMAGE_ID = 4014;


HUD::HUD(Manager *newMan)
{
	man = newMan;
	posBufObjHUD = 0;
	colorBufObjHUD = 0;
	GrndTexBuffObj = 0;
	GIndxBuffObj = 0;
	posBufObjMenu = 0;
	posBufObjSideHUD = 0;
	posBufObjFace = 0;
	posBufObjHeart = 0;
	posBufObjDam = 0;
	indxBuffObjMenu = 0;
	indxBuffObjSideHUD = 0;
	textBuffMenu = 0;
	textBuffSideHud = 0;
	on = false;
	homeScreenOn = true;
	deadScreenOn = false;
}

void HUD::initHUD(TextureLoader *texLoader)
{
	ModelTrans.useModelViewMatrix();
	ModelTrans.loadIdentity();
	initText2D( "Holstein.DDS" );

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

	texLoader->LoadTexture((char *)"assets/newHud.bmp", HUD_ID);

  texLoader->LoadTexture((char *)"assets/deadScreen.bmp", DEAD_ID);
  texLoader->LoadTexture((char *)"assets/faceHUD.bmp", SICKFACE_ID);
  texLoader->LoadTexture((char *)"assets/faceHUD.bmp", HAPPYFACE_ID);
  texLoader->LoadTexture((char *)"assets/faceHUD.bmp", SADFACE_ID);
  texLoader->LoadTexture((char *)"assets/health2HUD.bmp", HEALTH_ID);
  texLoader->LoadTexture((char *)"assets/swordsHUD.bmp", DAMAGE_ID);
  texLoader->LoadTexture((char *)"assets/faceHUD.bmp", MERCFACE_ID);
  texLoader->LoadTexture((char *)"assets/deadFaceHUD.bmp", DEADFACE_ID);

  // sideHUD texture load
  tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile("assets/sideHUD4.png");
  sideHud = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

  // topHUD texture load
  bmp = tdogl::Bitmap::bitmapFromFile("assets/newHud2.png");
  topHud = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);
  
  // face texture load
  bmp = tdogl::Bitmap::bitmapFromFile("assets/faceBlank.png");
  face = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

  // sickFace texture load
  bmp = tdogl::Bitmap::bitmapFromFile("assets/sickFaceBlank.png");
  sickFace = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

  // sickFace texture load
  bmp = tdogl::Bitmap::bitmapFromFile("assets/deadFaceBlank.png");
  deadFace = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

  // heart texture load
  bmp = tdogl::Bitmap::bitmapFromFile("assets/heart2Plane.png");
  heart = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);
  // swords texture load
  bmp = tdogl::Bitmap::bitmapFromFile("assets/swordsBlank2.png");
  swords = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

	GLfloat vert[] = {
		0, 0, 1.0f,
		0, 64.0f, 1.0f,
		1024.0f, 64.0f, 1.0f,
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

void HUD::initSideHud(TextureLoader *texLoader)
{
  texLoader->LoadTexture((char *)"assets/sideHUD2.bmp", SIDEHUD_ID);

  GLfloat homeVerts[] = {
    800.0f, 78.0f, 1.0f,
    800.0f, 750.0f, 1.0f,
    1024.0f, 750.0f, 1.0f,
    1024.0f, 78.0f, 1.0f 
  };

  glGenBuffers(1, &posBufObjSideHUD);
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjSideHUD);
  glBufferData(GL_ARRAY_BUFFER, sizeof(homeVerts), homeVerts, GL_STATIC_DRAW);

  GLfloat faceVerts[] = {
  	820.0f, 113.0f, 1.0f,
    820.0f, 145.0f, 1.0f,
    852.0f, 145.0f, 1.0f,
    852.0f, 113.0f, 1.0f
  };

  glGenBuffers(1, &posBufObjFace);
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjFace);
  glBufferData(GL_ARRAY_BUFFER, sizeof(faceVerts), faceVerts, GL_STATIC_DRAW);

  GLfloat heartVerts[] = {
  	820.0f, 167.0f, 1.0f,
    820.0f, 199.0f, 1.0f,
    852.0f, 199.0f, 1.0f,
    852.0f, 167.0f, 1.0f
  };

  glGenBuffers(1, &posBufObjHeart);
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjHeart);
  glBufferData(GL_ARRAY_BUFFER, sizeof(heartVerts), heartVerts, GL_STATIC_DRAW);

  GLfloat damVerts[] = {
  	820.0f, 207.0f, 1.0f,
    820.0f, 239.0f, 1.0f,
    852.0f, 239.0f, 1.0f,
    852.0f, 207.0f, 1.0f
  };

  glGenBuffers(1, &posBufObjDam);
  glBindBuffer(GL_ARRAY_BUFFER, posBufObjDam);
  glBufferData(GL_ARRAY_BUFFER, sizeof(damVerts), damVerts, GL_STATIC_DRAW);


  static GLfloat GrndTex[] = {
      0, 1, // back
      0, 0,
      1, 0,
      1, 1 };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    g_GiboLen = 6;
    glGenBuffers(1, &textBuffSideHud);
    glBindBuffer(GL_ARRAY_BUFFER, textBuffSideHud);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &indxBuffObjSideHUD);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxBuffObjSideHUD);
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

void HUD::changeSideHudPage(int page)
{
	if(page == 1)
	{
		if(man -> mercs.size() > currentPage * 4)
		{
			mercOffset += 4;
			printf("Changed page next\n");
			++currentPage;
		}
	}
	else
	{
		if(man -> mercs.size() > 4 && currentPage > 1)// && man ->mercs.size() < currentPage * 4)
		{
			mercOffset -= 4;
			printf("Changed page back\n");
			--currentPage;
		}
	}
}

void HUD::drawHud(Camera * camera, int width, int height)
{
  glUseProgram(pid);
  MatrixStack proj;
  proj.pushMatrix();
  camera->applyProjectionMatrix(&proj);
  glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
  proj.pushMatrix();
	// enableBuff(h_vertPos, h_aTexCoord);
  // enableBuff(posBufObjHUD, HUD_ID);
  enableBuff(posBufObjHUD, topHud->object(), GrndTexBuffObj);

	mat4 Ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
 
	glDisable(GL_DEPTH_TEST); // Disable the Depth-testing
	 
	glm::mat4 _guiMVP;
	_guiMVP = Ortho * glm::mat4(1.0f); // Identity Matrix
	 
	glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
	
	glEnable(GL_DEPTH_TEST); // Enable the Depth-testing
	disableBuff();
  proj.popMatrix();
  glUseProgram(0);
}

void HUD::drawSideHud(Camera * camera, int width, int height)
{
  glUseProgram(pid);
  MatrixStack proj;
  proj.pushMatrix();
  camera->applyProjectionMatrix(&proj);
  glUniformMatrix4fv( h_ProjMatrix, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
  proj.pushMatrix();
  // enableBuff(h_vertPos, h_aTexCoord);

  enableBuff(posBufObjSideHUD, sideHud->object(), textBuffSideHud);

  mat4 Ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
 
  glDisable(GL_DEPTH_TEST); // Disable the Depth-testing
   
  glm::mat4 _guiMVP;
  _guiMVP = Ortho * glm::mat4(1.0f); // Identity Matrix
   
  glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
  disableBuff();


  glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));

  string data;
  Mercenary tempMerc;
  int remain = man -> mercs.size() % 4;

//for(mercOffset; mercOffset < man -> mercs.size(); ++mercOffset)
for(mercOffset; mercOffset < (currentPage * 4) && mercOffset < man -> mercs.size(); ++mercOffset)
{
	glUseProgram(pid);
	tempMerc = man -> mercs[mercOffset];

	mat4 trans = glm::translate(glm::mat4(1.0f), vec3(0.0f, offset, 0.0f));
	_guiMVP = trans * _guiMVP;
	glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(_guiMVP));

  if(tempMerc.currHealth == 0)
  {
    enableBuff(posBufObjFace, deadFace->object(), textBuffSideHud);
  }
  else if(tempMerc.sick)
  {
    enableBuff(posBufObjFace, sickFace->object(), textBuffSideHud);
  }
  else
  {
    enableBuff(posBufObjFace, face->object(), textBuffSideHud);
  }
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
	disableBuff();

	enableBuff(posBufObjHeart, heart->object(), textBuffSideHud);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
	disableBuff();

	enableBuff(posBufObjDam, swords->object(), textBuffSideHud);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); // draw first object
	disableBuff();
	offset = -0.4;

	data = tempMerc.firstName; //+ " " + man -> mercs[i].lastName;
	printText2D(data.c_str(), 675, stringOffset, 12);
	// data = "the " + tempMerc.title;
	// printText2D(data.c_str(), 665, stringOffset - 15, 8);

	data = to_string(static_cast<long long int>(tempMerc.currHealth)) + "/" + to_string(static_cast<long long int>(tempMerc.maxHealth));
	printText2D(data.c_str(), 685, stringOffset - 40, 12);

	data = to_string(static_cast<long long int>(tempMerc.calcDamage()));
	printText2D(data.c_str(), 685, stringOffset - 70, 12);

	stringOffset -= 120;
}
mercOffset = -4 + currentPage * 4;
offset = 0;
stringOffset = 495;
glEnable(GL_DEPTH_TEST); // Enable the Depth-testing
proj.popMatrix();

	//132 offset I think?
}

void HUD::enableBuff(GLuint posBufObjA, int ID, GLuint newGrdTxt) {
	glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);

  GLSL::enableVertexAttribArray(h_vertPos); //position
  if(homeScreenOn || deadScreenOn)
  {
  	glBindBuffer(GL_ARRAY_BUFFER, posBufObjMenu);
  }
  else
  {
  	glBindBuffer(GL_ARRAY_BUFFER, posBufObjA);
    
  }
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  if(homeScreenOn)
  {
  	glBindTexture(GL_TEXTURE_2D, HOME_ID);
  }
  else if(deadScreenOn)
  {
    glBindTexture(GL_TEXTURE_2D, DEAD_ID);
  }
  else
  {
  	glBindTexture(GL_TEXTURE_2D, ID);
  }

  GLSL::enableVertexAttribArray(h_aTexCoord);

  if(homeScreenOn || deadScreenOn)
  {
  	glBindBuffer(GL_ARRAY_BUFFER, textBuffMenu);
  }
  else
  {
  	glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
  }
  glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT ,GL_FALSE, 0, 0);

  if(homeScreenOn || deadScreenOn)
  {
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxBuffObjMenu);
  }
  else
  {
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
  }
}

void HUD::disableBuff() {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisable(GL_TEXTURE_2D);
}