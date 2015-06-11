#include "tavern.h"
#include <time.h>

int TAV_CRATE_ID = 5000;
int TAV_DOOR_ID = 5001;
int TAV_TABLECHAIR_ID = 5002;
int TAV_TABLE_ID = 5003;
int TAV_BARRELS_ID = 5004;
int TAV_MIRRORFRAME_ID = 5005;
int TAV_WINDOW_ID = 5006;
int TAV_STAIRS_ID = 5007;
int TAV_BARSTAND_ID = 5008;
int TAV_LANDLORD_ID = 5500;
int FIREPIT_BAKED = 5600;
int TAV_LUMBERJACK_ID = 5700;
int TAV_SAMURAI_ID = 5800;
int TAV_SHELF_ID = 5900;
int TAV_MARBLE_ID = 6000;
int TAV_BRANCHES_ID = 6100;
int TAV_ROOF_ID = 6200;
int TAV_TORCH_ID = 6300;
int TAV_PLANK_ID = 6400;
int TAV_ROCK_ID = 6500;
int TAV_DIRT_ID = 6600;
int MINOTAUR_EMBLEM_ID = 4400;
int CENTAUR_EMBLEM_ID = 4500;
int DRAGON_EMBLEM_ID = 4600;
int PEGASUS_EMBLEM_ID = 4700;
int PHOENIX_EMBLEM_ID = 4800;
int HYDRA_EMBLEM_ID = 4900;
int HORSE_ID = 22100;

// Obj3dContainer containers[std::extent<decltype(tavObjFiles)>::value];

int TURKEY_NUM;

Tavern::Tavern() :
    sam(CharDae("not_a_file", 0, 0.0f, 0))
{
	doorLoc = vec3(7.5, 1.35, -23);
	beerLoc = vec3(35.0, 1.0, -35.0);
	foodLoc = vec3(30, 1.5, -30);
}

void Tavern::init(Materials *newMatSetter, FrustumCull *newCuller, ProjectMeshes *newData)
{
	matSetter = newMatSetter;
	fCuller = newCuller;
	meshData = newData;
	curTime = 0;
}

//between [1, limit]
int Tavern::getRandInt(int limit)
{
	return rand() % limit + 1;
}
//between [0, limit]
float Tavern::getRandFloat(float limit)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / limit);
}

//index is the index of buffer info in tavernMeshes, vectMesh is the vector which stores the mesh for the obj3d
void Tavern::addTavernItem(int index, int vectMesh, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
	if (vectMesh == 1) {
		Obj3d temp(&((*meshData).tavMeshes[index]), scale, trans, rot);
		tavernItems.push_back(temp);
	}
	else if (vectMesh == 2) {
		Obj3d temp(&((*meshData).otherMeshes[index]), scale, trans, rot);
		tavernItems.push_back(temp);
	}
	else if (vectMesh == 3) {
		Obj3d temp(&((*meshData).pplMeshes[index]), scale, trans, rot);
		tavernItems.push_back(temp);
	}
}

//index is the index of buffer info in tavernMeshes
void Tavern::addTavernCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
    vector<Obj3d> bodyParts;
	Obj3d torso(&((*meshData).pplMeshes[index]), scale, trans, rot);
    // TODO our arm's translation should be based off rotation
	//Obj3d arm(&(tavernMeshes[CUBE]), glm::vec3(0.1, 0.5, 0.1), trans + glm::vec3(-0.3, 0.6, 0), rot);
    //arm.preTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.4, 0.0));
    bodyParts.push_back(torso);
    //bodyParts.push_back(arm);
    
    Mercenary* merc = new Mercenary(bodyParts);
    merc->dae = NULL;

	tavernCharacters.push_back(*merc);
}

void Tavern::loadBufferData(TextureLoader* texLoader)
{
	//load textures
	texLoader->LoadTexture((char *)"assets/tavern/crateTex.bmp", TAV_CRATE_ID);
	texLoader->LoadTexture((char *)"assets/tavern/landlordTex.bmp", TAV_LANDLORD_ID);
	texLoader->LoadTexture((char *)"assets/tavern/FirepitBaked.bmp", FIREPIT_BAKED);
	texLoader->LoadTexture((char *)"assets/tavern/lumberjackTex.bmp", TAV_LUMBERJACK_ID);
	texLoader->LoadTexture((char *)"assets/tavern/samuraiTex.bmp", TAV_SAMURAI_ID);
	texLoader->LoadTexture((char *)"assets/tavern/bookshelfTex.bmp", TAV_SHELF_ID);
	texLoader->LoadTexture((char *)"assets/tavern/marbletopTex.bmp", TAV_MARBLE_ID);
	texLoader->LoadTexture((char *)"assets/tavern/branchTex.bmp", TAV_BRANCHES_ID);
	texLoader->LoadTexture((char *)"assets/tavern/roofTex.bmp", TAV_ROOF_ID);
	texLoader->LoadTexture((char *)"assets/tavern/torchTex.bmp", TAV_TORCH_ID);
	texLoader->LoadTexture((char *)"assets/tavern/plankTex.bmp", TAV_PLANK_ID);
	texLoader->LoadTexture((char *)"assets/tavern/rockTex.bmp", TAV_ROCK_ID);
	texLoader->LoadTexture((char *)"assets/tavern/dirtTex.bmp", TAV_DIRT_ID);
	texLoader->LoadTexture((char *)"assets/tavern/minotaurTex2.bmp", MINOTAUR_EMBLEM_ID);
	texLoader->LoadTexture((char *)"assets/tavern/centaurTex2.bmp", CENTAUR_EMBLEM_ID);
	texLoader->LoadTexture((char *)"assets/tavern/dragonTex2.bmp", DRAGON_EMBLEM_ID);
	texLoader->LoadTexture((char *)"assets/tavern/pegasusTex2.bmp", PEGASUS_EMBLEM_ID);
	texLoader->LoadTexture((char *)"assets/tavern/phoenixTex2.bmp", PHOENIX_EMBLEM_ID);
	texLoader->LoadTexture((char *)"assets/tavern/hydraTex2.bmp", HYDRA_EMBLEM_ID);
	texLoader->LoadTexture((char *)"assets/tavern/door/doorDiffuse.bmp", TAV_DOOR_ID);
	texLoader->LoadTexture((char *)"assets/tavern/table/tableDiffuse.bmp", TAV_TABLECHAIR_ID);
	texLoader->LoadTexture((char *)"assets/tavern/table/tableDiffuse.bmp", TAV_TABLE_ID);
	texLoader->LoadTexture((char *)"assets/tavern/barrel/barrelDiffuse.bmp", TAV_BARRELS_ID);
	texLoader->LoadTexture((char *)"assets/tavern/table/tableDiffuse1.bmp", TAV_MIRRORFRAME_ID);
	texLoader->LoadTexture((char *)"assets/tavern/mirror/village1.bmp", TAV_WINDOW_ID);
	texLoader->LoadTexture((char *)"assets/horse/tex/tex_00.bmp", HORSE_ID);
	texLoader->LoadTexture((char *)"assets/tavern/table/tableDiffuse1.bmp", TAV_STAIRS_ID);
	texLoader->LoadTexture((char *)"assets/tavern/barstand/barstand.bmp", TAV_BARSTAND_ID);
}

void Tavern::createTable1(glm::vec3 initLoc, float ang)
{
	//index for chair translations
	int multInd[] = {-1,  1,
	                  1,  1,
	                 -1, -1,
	                  1, -1,
	                  1, -1,
	                  1,  1,
	                 -1,  1,
	                 -1, -1};

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(RECT_TABLE, 1, glm::vec3(1.6, 1.6, 1.6), glm::vec3(initLoc.x, initLoc.y + 0.5, initLoc.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLE_ID);

	int numChairs = 2;
	glm::mat4 addedRot = glm::rotate(glm::mat4(1.0f), ang + 180, glm::vec3(0, 1.0f, 0));

	int start = (ang == 0) ? 0 : 8;
	for (int iter = start; iter < 4 + start; iter += 2) {
		addTavernItem(CHAIR, 1, glm::vec3(0.60, 0.60, 0.60), glm::vec3(initLoc.x + multInd[iter + 0], 0.6, initLoc.z + multInd[iter + 1]), rot);
		tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
		addTavernItem(CHAIR, 1, glm::vec3(0.60, 0.60, 0.60), glm::vec3(initLoc.x + multInd[iter + 4], 0.6, initLoc.z + multInd[iter + 5]), addedRot);
		tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	}
}

void Tavern::createPillar(glm::vec3 initLoc, bool north)
{
	float ang = 90.0f;
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1.0f));
	addTavernItem(PLANK, 1, glm::vec3(12, 15, 2.5), glm::vec3(initLoc.x, initLoc.y + 5.0f, initLoc.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_PLANK_ID);
	
	if (north) 
	{
		ang = -45;
		rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
		addTavernItem(TORCH, 1, glm::vec3(0.4, 0.4, 0.4), glm::vec3(initLoc.x, initLoc.y + 1.6, initLoc.z - 0.2), rot);
		
	}
	else 
	{
		ang = 45;
		rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
		addTavernItem(TORCH, 1, glm::vec3(0.4, 0.4, 0.4), glm::vec3(initLoc.x, initLoc.y + 1.6, initLoc.z + 0.2), rot);
	}
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TORCH_ID);
}

void Tavern::createFirePlace(glm::vec3 init)
{
	addTavernItem(FIREPLACE, 1, glm::vec3(1.2, 1.2, 1.2), glm::vec3(init.x, 0.85f, init.z), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(FIREPIT_BAKED);

	addTavernItem(TURKEY, 1, glm::vec3(.35, .35, .35), glm::vec3(init.x, 1.65f, init.z), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 14;
	TURKEY_NUM = tavernItems.size() - 1;
}

void Tavern::newEmblem()
{
	int emblemTex = getRandInt(6) * 100 + 4300;
	tavernItems[wallEmblem].loadTextureCoor(emblemTex);
	tavernItems[wallEmblem - 1].loadTextureCoor(emblemTex);
	tavernItems[wallEmblem - 2].loadTextureCoor(emblemTex);
	tavernItems[wallEmblem - 3].loadTextureCoor(emblemTex);
}

void Tavern::createEmblems()
{
	int emblemTex = getRandInt(6) * 100 + 4300;
	// int emblemTex = WOLF_EMBLEM_ID;
	glm::vec3 scaler = glm::vec3(0.75, 0.75, 0.75);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (const float)90, glm::vec3(0, 1.0f, 0));
	addTavernItem(CRATE, 2, scaler, glm::vec3(6.85, 3.8, -23.0), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(emblemTex);
	addTavernItem(CRATE, 2, scaler, glm::vec3(39.15, 3.8, -23.5), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(emblemTex);

	addTavernItem(CRATE, 2, scaler, glm::vec3(23.05, 3.8, -11.35), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(emblemTex);
	addTavernItem(CRATE, 2, scaler, glm::vec3(23.05, 3.8, -35.65), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(emblemTex);
	wallEmblem = tavernItems.size() - 1;
}

void Tavern::loadTavernMeshes(TextureLoader* texLoader)
{
	srand(time(NULL));
	float ang;
	glm::mat4 rot;

	loadBufferData(texLoader);

	//2nd floor
	addTavernItem(CUBE, 1, glm::vec3(28, 0.5, 8.0), glm::vec3(25, 7, -12), glm::mat4(1.0f));
	addTavernItem(CUBE, 1, glm::vec3(28, 0.5, 8.0), glm::vec3(25, 7, -35), glm::mat4(1.0f));
	addTavernItem(CUBE, 1, glm::vec3(11.0, 0.5, 28.0), glm::vec3(7, 7, -25), glm::mat4(1.0f));
	addTavernItem(CUBE, 1, glm::vec3(11.0, 0.5, 28.0), glm::vec3(39, 7, -25), glm::mat4(1.0f));

	//roof
	ang = 45;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(ROOF, 1, glm::vec3(28.0, 5.0, 28.0), glm::vec3(25, 9, -12), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROOF_ID);
	ang = -45;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(ROOF, 1, glm::vec3(28, 5.0, 28.0), glm::vec3(25, 9, -35), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROOF_ID);

	//door
	addTavernItem(DOOR, 1, glm::vec3(1.5, 1.5, 1.5), glm::vec3(7.5, 1.35, -23), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_DOOR_ID);

	//bar stools
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(11, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(12, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(12.9, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(13.5, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(15, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(16.2, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(17.5, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(18.2, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);
	addTavernItem(STOOL, 1, glm::vec3(0.35, 0.32, 0.35), glm::vec3(18.9, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TABLECHAIR_ID);

	//Stairs
	addTavernItem(STAIRS, 1, glm::vec3(3.0, 4.3, 3.0), glm::vec3(22.6, 4.35, -18.0), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_STAIRS_ID);

	//Table by door #1
	createTable1(glm::vec3(9.0, 0.0, -26.0), 90);
	//Table #2
	createTable1(glm::vec3(12.0, 0.0, -29.0), 0);
	//Table #3
	createTable1(glm::vec3(15.5, 0.0, -29.0), 0);
	//Table #4
	createTable1(glm::vec3(19.0, 0.0, -29.0), 0);
	//Table #5
	createTable1(glm::vec3(22.0, 0, -26.5), 90);
	//Table #6
	createTable1(glm::vec3(22.0, 0, -23), 90);

	//Barrels
	addTavernItem(BARRELS, 1, glm::vec3(1.5, 1.5, 1.5), glm::vec3(8.8, 0.7, -15.0), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_BARRELS_ID);
	//Barrels by stairs to prevent players from going under stairs
	addTavernItem(BARRELS, 1, glm::vec3(1.5, 1.5, 1.5), glm::vec3(22.6, 0.7, -18.0), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_BARRELS_ID);

	//Mirror
	addTavernItem(MIRRORFRAME, 1, glm::vec3(1.5, 1.5, 1.5), glm::vec3(7.6, 2.0, -19.1), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_MIRRORFRAME_ID);
	addTavernItem(MIRROR, 1, glm::vec3(1.5, 1.5, 1.5), glm::vec3(7.6, 2.0, -19.1), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_WINDOW_ID);

	//adding bookshelves
	ang = 180;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(BOOKSHELF, 1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(12.4, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, 1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(13.7, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, 1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(15, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, 1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(16.3, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, 1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(17.6, 1.74, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);

	//Bar Stand
	addTavernItem(BARSTAND, 1, glm::vec3(5.3, 5.0, 5.0), glm::vec3(15.5, 0.6, -15), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_BARSTAND_ID);
	//Jesus Holder
	addTavernItem(JESUSHOLDER, 1, glm::vec3(0.8, 1.1, 1.0), glm::vec3(21.53, .72, -15.5), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_MIRRORFRAME_ID);

	//Add Characters
	addTavernCharacter(SAMURAI, glm::vec3(1, 1, 1), glm::vec3(15.5, 1.1, -25.0), glm::mat4(1.0f));
	tavernCharacters[tavernCharacters.size() - 1].meshes[0].loadTextureCoor(TAV_SAMURAI_ID);

	addTavernCharacter(SAMURAI, glm::vec3(1, 1, 1), glm::vec3(17.5, 1.1, -23.0), glm::mat4(1.0f));
	tavernCharacters[tavernCharacters.size() - 1].meshes[0].loadTextureCoor(TAV_SAMURAI_ID);

	addTavernCharacter(SAMURAI, glm::vec3(1, 1, 1), glm::vec3(13.5, 1.1, -23.0), glm::mat4(1.0f));
	tavernCharacters[tavernCharacters.size() - 1].meshes[0].loadTextureCoor(TAV_SAMURAI_ID);

	//fireplace and roasting
	createFirePlace(glm::vec3(15.5, 1.5, -23.0));
	createEmblems();

    // try some assimp stuff
    sam = CharDae("assets/characters/bartender.dae", 5500, 1.2f, 7);
    fellowTraveler = new CharDae("assets/characters/lumberjack.dae", 5700, 1.2f, 8);
    fellowTraveler2 = new CharDae("assets/characters/lumberjack.dae", 5700, 1.2f, 8);
}

void Tavern::enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf) {
  GLSL::enableVertexAttribArray(h_vertPos); //position
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

  GLSL::enableVertexAttribArray(h_vertNor); //normals
  glBindBuffer(GL_ARRAY_BUFFER, norBuf);
  glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
}

void Tavern::disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord) {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_vertNor);
  GLSL::disableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisable(GL_TEXTURE_2D);
}

void Tavern::enableTextureBuffer(GLint h_aTexCoord, GLuint texBuf, int id)
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id);
  //mipmap
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  
  GLSL::enableVertexAttribArray(h_aTexCoord);

  glBindBuffer(GL_ARRAY_BUFFER, texBuf);
  glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Tavern::drawTavern(GLint h_ModelMatrix, GLint h_vertPos, 
                GLint h_vertNor, GLint h_aTexCoord, double ltime,
                GLint h_boneFlag, GLint h_boneIds,
                GLint h_boneWeights, GLint h_boneTransforms)
{
	applyTurkeySpin(ltime);
	for (int iter = 0; iter < tavernItems.size(); iter++) 
	{
		//set a material
		if (tavernItems[iter].materialNdx != -1) {
			(*matSetter).setMaterial(tavernItems[iter].materialNdx);
		}
		else {
			(*matSetter).setMaterial(9);
		}
		enableBuff(h_vertPos, h_vertNor, (*tavernItems[iter].cont).posBuf, (*tavernItems[iter].cont).norBuf, (*tavernItems[iter].cont).indBuf);
		if (tavernItems[iter].hasTexture) {
			enableTextureBuffer(h_aTexCoord, tavernItems[iter].texBuf, tavernItems[iter].textureNdx);
		}
		//decide whether to cull
		if ((*fCuller).checkCull(tavernItems[iter])) {
			tavernItems[iter].draw(h_ModelMatrix);
		}
		disableBuff(h_vertPos, h_vertNor, h_aTexCoord);
	}
	
/*	for (int iter = 0; iter < tavernCharacters.size(); iter++) {
        for(int meshIter = 0; meshIter < tavernCharacters[iter].meshes.size(); meshIter++) {
        	if ((*fCuller).checkCull(tavernCharacters[iter].meshes[meshIter])) {
			    enableBuff(h_vertPos, h_vertNor, 
                    (*tavernCharacters[iter].meshes[meshIter].cont).posBuf, 
                    (*tavernCharacters[iter].meshes[meshIter].cont).norBuf, 
                    (*tavernCharacters[iter].meshes[meshIter].cont).indBuf);
			    if (tavernCharacters[iter].meshes[meshIter].hasTexture) {
			    	enableTextureBuffer(h_aTexCoord, 
                        tavernCharacters[iter].meshes[meshIter].texBuf, 
                        tavernCharacters[iter].meshes[meshIter].textureNdx);
			    }
			    tavernCharacters[iter].draw(h_ModelMatrix, meshIter);
			    disableBuff(h_vertPos, h_vertNor, h_aTexCoord);
			}
        }
	}*/
}

void Tavern::drawTavernMercs(GLint h_ModelMatrix, GLint h_vertPos, 
                GLint h_vertNor, GLint h_aTexCoord, double ltime,
                GLint h_boneFlag, GLint h_boneIds,
                GLint h_boneWeights, GLint h_boneTransforms, GLint h_texFlag,
                GLint h_boneIds2, GLint h_boneWeights2)
{
    for (int iter = 0; iter < tavernCharacters.size(); iter++) {
        static int animSelect = 0;
        animSelect++;
        if(animSelect > 30) {
            animSelect = 0;
        }

        if (tavernCharacters[iter].dae == NULL) {
            tavernCharacters[iter].initDae();
        }
        if(!tavernCharacters[iter].dae->isAnimating()) {
            tavernCharacters[iter].dae->startAnimation("idle");
        }
        tavernCharacters[iter].dae->position = tavernCharacters[iter].meshes[0].pos;
        tavernCharacters[iter].dae->position.y -= 1;
        tavernCharacters[iter].dae->scale = glm::vec3(1.0f, 1.0f, 1.0f);
        //enableTextureBuffer(h_aTexCoord, tavernCharacters[iter].dae->texBuf, TAV_SAMURAI_ID);

        tavernCharacters[iter].dae->drawChar(h_ModelMatrix, h_vertPos, h_vertNor, h_aTexCoord, 
                h_boneFlag, h_boneIds, h_boneWeights, h_boneTransforms, ltime, h_texFlag,
                h_boneIds2, h_boneWeights2);
    }

    int barKeepAnimChoice = rand() % 10;

    if (!sam.isAnimating()) {
        sam.startAnimation("idle");
        if (barKeepAnimChoice > 6) {
            sam.startAnimation("run");    
        }
    }

    sam.position = glm::vec3(15, 0, -13.5);
    sam.rotate = 180;
    sam.drawChar(h_ModelMatrix, h_vertPos, h_vertNor, h_aTexCoord, 
                h_boneFlag, h_boneIds, h_boneWeights, h_boneTransforms, ltime, h_texFlag,
                h_boneIds2, h_boneWeights2);

    if (!fellowTraveler->isAnimating()) {
    	if (barKeepAnimChoice >= 6) {
        fellowTraveler->startAnimation("punch");
    	}
        else if (barKeepAnimChoice > 3) {
            fellowTraveler->startAnimation("run");    
        }
        else {
            fellowTraveler->startAnimation("idle");    
        }
    }

    fellowTraveler->position = glm::vec3(15.5, 0, -16.8);
    fellowTraveler->rotate = 0;
    fellowTraveler->drawChar(h_ModelMatrix, h_vertPos, h_vertNor, h_aTexCoord, 
                             h_boneFlag, h_boneIds, h_boneWeights, h_boneTransforms, ltime, h_texFlag,
                             h_boneIds2, h_boneWeights2);

    if (!fellowTraveler2->isAnimating()) {
    	if (barKeepAnimChoice >= 6) {
        fellowTraveler2->startAnimation("idle");
    	}
        else if (barKeepAnimChoice > 3) {
            fellowTraveler2->startAnimation("run");    
        }
        else {
            fellowTraveler2->startAnimation("punch");    
        }
    }

    fellowTraveler2->position = glm::vec3(13.5, 0, -16.4);
    fellowTraveler2->rotate = 25;
    fellowTraveler2->drawChar(h_ModelMatrix, h_vertPos, h_vertNor, h_aTexCoord, 
                             h_boneFlag, h_boneIds, h_boneWeights, h_boneTransforms, ltime, h_texFlag,
                             h_boneIds2, h_boneWeights2);
}

void Tavern::applyTurkeySpin(double ltime)
{
	float ang = 90;
	mat4 constRot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	curTime += 0.1;
	mat4 newRot = glm::rotate(glm::mat4(1.0f), curTime, glm::vec3(0, 0, 1.0f));
	tavernItems[TURKEY_NUM].rot = constRot * newRot;
}

vec3 Tavern::getDoorLoc()
{
	return doorLoc;
}

vec3 Tavern::getBeerLoc()
{
	return beerLoc;
}

vec3 Tavern::getFoodLoc()
{
	return foodLoc;
}

void Tavern::showMercsForSale()
{
    cout << "Mercs for sale: " + to_string(static_cast<long long int>(tavernCharacters.size())) + "\n";
    for(int i = 0; i < tavernCharacters.size(); i++)
    {
        cout << "mercenary " + to_string(static_cast<long long int>(i)) + ": ";
        tavernCharacters[i].printDetails();
    }
}
