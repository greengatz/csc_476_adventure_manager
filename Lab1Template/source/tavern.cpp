#include "tavern.h"
#include <time.h>

#define CUBE 0
#define DOOR 1
#define BOOKSHELF 2
#define STOOL 3
#define CHAIR 4
#define RECT_TABLE 5
#define CIR_TABLE 6
#define MUG 7
#define BOTTLE 8
#define TORCH 9
#define BARREL 10
#define STICK 11
#define POLE 12
#define FIREPLACE 13
#define TABLEWARE 14
#define TURKEY 15
#define LANDLORD 16
#define LUMBERJACK 17
#define SAMURAI 18 // being given a box for an arm right now
#define ROCK 19
#define CRATE 20
#define BALUSTRADE 21
#define ROOF 22
#define PLANK 23

#define NUMFILES 24

const string tavObjFiles[] = {"assets/tavern/cube.obj",
                       	      "assets/tavern/door.obj",
                              "assets/tavern/bookshelf.obj",
                              "assets/tavern/stool.obj",
                              "assets/tavern/chair.obj",
                              "assets/tavern/table.obj",
                              "assets/tavern/diningtable.obj",
                       	      "assets/tavern/mug.obj",
                       	      "assets/tavern/bottle.obj",
                       	      "assets/tavern/torch.obj",
                              "assets/tavern/barrel.obj",   ///this is the barrel
                              "assets/tavern/stick.obj",
                              "assets/tavern/pole.obj",
                              "assets/tavern/fireplace.obj",
                              "assets/tavern/tableware.obj",
                              "assets/tavern/turkey.obj",
                      	      "assets/tavern/landlord.obj",
                   		      "assets/tavern/lumberjack.obj",
               			      "assets/tavern/samurai.obj",
               			      "assets/tavern/rock.obj",
               			      "assets/tavern/box.obj",
               			      "assets/tavern/balustrade.obj", //currently a cube until can find a good balustrade.obj
               			      "assets/tavern/roof.obj",
               			      "assets/tavern/plank.obj"
               			};

int TAV_CRATE_ID = 5000;
int TAV_LANDLORD_ID = 5500;
int TAV_LOG_ID = 5600;
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

Obj3dContainer containers[std::extent<decltype(tavObjFiles)>::value];

int TURKEY_NUM;

Tavern::Tavern()
{
	doorLoc = vec3(7.5, 1.35, -23);
	beerLoc = vec3(35.0, 1.0, -35.0);
	foodLoc = vec3(30, 1.5, -30);
}

void Tavern::init(Materials *newMatSetter, FrustumCull *newCuller)
{
	matSetter = newMatSetter;
	fCuller = newCuller;
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

void Tavern::addTavernMesh(const string filename, bool noNorms)
{
	Obj3dContainer temp;
	temp.loadIntoTinyOBJ(filename);
	temp.initBuffers(noNorms);
	tavernMeshes.push_back(temp);
}

//index is the index of buffer info in tavernMeshes
void Tavern::addTavernItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
	Obj3d temp(&(tavernMeshes[index]), scale, trans, rot);
	tavernItems.push_back(temp);
}

//index is the index of buffer info in tavernMeshes
void Tavern::addTavernCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
    vector<Obj3d> bodyParts;
	Obj3d torso(&(tavernMeshes[index]), scale, trans, rot);
    // TODO our arm's translation should be based off rotation
	Obj3d arm(&(tavernMeshes[CUBE]), glm::vec3(0.1, 0.5, 0.1), trans + glm::vec3(-0.3, 0.6, 0), rot);
    arm.preTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.4, 0.0));
    bodyParts.push_back(torso);
    bodyParts.push_back(arm);
    
	tavernCharacters.push_back(*(new Mercenary(bodyParts)));
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
	addTavernItem(RECT_TABLE, glm::vec3(1.6, 1.6, 1.6), glm::vec3(initLoc.x, initLoc.y + 0.5, initLoc.z), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 6;
	int numChairs = getRandInt(2);
	glm::mat4 addedRot = glm::rotate(glm::mat4(1.0f), ang + 180, glm::vec3(0, 1.0f, 0));

	if (numChairs == 2) { //4 chairs at table
		int start = (ang == 0) ? 0 : 8;
		for (int iter = start; iter < 4 + start; iter += 2) {
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[iter + 0], initLoc.y + 0.5, initLoc.z + multInd[iter + 1]), rot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[iter + 4], initLoc.y + 0.5, initLoc.z + multInd[iter + 5]), addedRot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
		}
	}
	else //3 chairs
	{
		int start = (ang == 0) ? 0 : 8;

		float space = getRandFloat(2) - 1.0;
		int rotEdit = getRandInt(90) - 45;

		if (ang == 0) {
			glm::mat4 changedRot = glm::rotate(glm::mat4(1.0f), rotEdit + ang, glm::vec3(0, 1.0f, 0));
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + space, initLoc.y + 0.5, initLoc.z + multInd[start + 1]), changedRot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[start + 4], initLoc.y + 0.5, initLoc.z + multInd[start + 5]), addedRot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[start + 6], initLoc.y + 0.5, initLoc.z + multInd[start + 7]), addedRot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
		}
		else {
			rotEdit -= 90;
			glm::mat4 changedRot = glm::rotate(glm::mat4(1.0f), (float)rotEdit, glm::vec3(0, 1.0f, 0));
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[start], initLoc.y + 0.5, initLoc.z + multInd[start + 1]), rot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[start + 2], initLoc.y + 0.5, initLoc.z + multInd[start + 3]), rot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
			addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + multInd[start + 6], initLoc.y + 0.5, initLoc.z + space), changedRot);
			tavernItems[tavernItems.size() - 1].materialNdx = 11;
		}
	}

	//stuff on the table
	if (getRandInt(3) > 1) {
		float x = getRandFloat(1) - 0.5;
		float z = getRandFloat(1) - 0.5;
		float allDir = getRandFloat(360);
		glm::mat4 newRot = glm::rotate(glm::mat4(1.0f), allDir, glm::vec3(0, 1.0f, 0));
		addTavernItem(MUG, glm::vec3(0.075, 0.075, 0.075), glm::vec3(initLoc.x + x, initLoc.y + 1.02, initLoc.z + z), newRot);
		tavernItems[tavernItems.size() - 1].materialNdx = 7;
		addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(initLoc.x + x + .25, initLoc.y + 1.12, initLoc.z + z), newRot);
		tavernItems[tavernItems.size() - 1].materialNdx = 8;

	}

	// if(getRandInt(2) > 1)
	// {
	// 	float x = getRandFloat(.5) - 0.25;
	// 	float z = getRandFloat(.5) - 0.25;
	// 	addTavernItem(TABLEWARE, glm::vec3(.5, .5, .5), glm::vec3(initLoc.x + x + .5, initLoc.y + 1.1, initLoc.z + z + .5), rot);
	// }
}

void Tavern::loadBufferData(TextureLoader* texLoader)
{
	for (int iter = 0; iter < NUMFILES; iter++) {
		// printf("iter is %d\n", iter);
		if (iter == BARREL)
			addTavernMesh(tavObjFiles[iter], true);
		else
			addTavernMesh(tavObjFiles[iter], false);
	}

	//load textures
	texLoader->LoadTexture((char *)"assets/tavern/crateTex.bmp", TAV_CRATE_ID);
	texLoader->LoadTexture((char *)"assets/tavern/landlordTex.bmp", TAV_LANDLORD_ID);
	texLoader->LoadTexture((char *)"assets/tavern/logTex.bmp", TAV_LOG_ID);
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
}

void Tavern::createPillar(glm::vec3 initLoc)
{
	// addTavernItem(CUBE, glm::vec3(0.2, 10.0, 0.2), initLoc, glm::mat4(1.0f));
	float ang = 90;
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1.0f));
	addTavernItem(PLANK, glm::vec3(12, 15, 2.5), glm::vec3(initLoc.x, initLoc.y + 5.0, initLoc.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_PLANK_ID);
	ang = 45;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(TORCH, glm::vec3(0.4, 0.4, 0.4), glm::vec3(initLoc.x, initLoc.y + 1.6, initLoc.z + 0.2), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TORCH_ID);
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1.0f));
	addTavernItem(TORCH, glm::vec3(0.4, 0.4, 0.4), glm::vec3(initLoc.x - 0.2, initLoc.y + 1.6, initLoc.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TORCH_ID);
	ang = -45;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(TORCH, glm::vec3(0.4, 0.4, 0.4), glm::vec3(initLoc.x, initLoc.y + 1.6, initLoc.z - 0.2), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TORCH_ID);
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1.0f));
	addTavernItem(TORCH, glm::vec3(0.4, 0.4, 0.4), glm::vec3(initLoc.x + 0.2, initLoc.y + 1.6, initLoc.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_TORCH_ID);
}

void Tavern::createFirePlace(glm::vec3 init)
{
	float ang;
	glm::mat4 rot;
	addTavernItem(POLE, glm::vec3(1.5, 1.2, 1.4), init, glm::mat4(1.0f));
	addTavernItem(FIREPLACE, glm::vec3(0.6, 0.6, 0.6), glm::vec3(init.x, -0.05, init.z + 0.3), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_LOG_ID);
	addTavernItem(FIREPLACE, glm::vec3(0.6, 0.6, 0.6), glm::vec3(init.x, -0.05, init.z - 0.3), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_LOG_ID);
	addTavernItem(FIREPLACE, glm::vec3(0.6, 0.6, 0.6), glm::vec3(init.x, 0.1, init.z), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_LOG_ID);
	addTavernItem(STICK, glm::vec3(1.0, 1.5, 1.0), glm::vec3(init.x - 1.3, 0.5, init.z - 0.075), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 7;
	addTavernItem(STICK, glm::vec3(1.0, 1.5, 1.0), glm::vec3(init.x + 1.3, 0.5, init.z - 0.075), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 7;

	//rock circle
	ang = 90;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x - 1.0, -0.05, init.z + 0.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x - 1.0, -0.05, init.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x - 1.0, -0.05, init.z - 0.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x + 1.0, -0.05, init.z + 0.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x + 1.0, -0.05, init.z), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x + 1.0, -0.05, init.z - 0.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1.0f));
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x - 0.5, -0.05, init.z + 0.7), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x, -0.05, init.z + 0.7), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x + 0.5, -0.05, init.z + 0.7), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x - 0.5, -0.05, init.z - 0.9), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x, -0.05, init.z - 0.9), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);
	addTavernItem(ROCK, glm::vec3(0.35, 0.35, 0.35), glm::vec3(init.x + 0.5, -0.05, init.z - 0.9), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROCK_ID);

	addTavernItem(CRATE, glm::vec3(0.95, 0.95, 0.95), glm::vec3(init.x, -0.87, init.z - 0.1), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_DIRT_ID);

	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(TURKEY, glm::vec3(.45, .45, .45), glm::vec3(init.x, 1.5, init.z), rot);
	TURKEY_NUM = tavernItems.size() -1;
}

void Tavern::loadTavernMeshes(TextureLoader* texLoader)
{
	srand(time(NULL));
	float ang;
	glm::mat4 rot;

	loadBufferData(texLoader);

	//2nd floor
	addTavernItem(CUBE, glm::vec3(28, 0.5, 8.0), glm::vec3(25, 7, -12), glm::mat4(1.0f));
	addTavernItem(CUBE, glm::vec3(28, 0.5, 8.0), glm::vec3(25, 7, -35), glm::mat4(1.0f));
	addTavernItem(CUBE, glm::vec3(11.0, 0.5, 28.0), glm::vec3(7, 7, -25), glm::mat4(1.0f));
	addTavernItem(CUBE, glm::vec3(11.0, 0.5, 28.0), glm::vec3(39, 7, -25), glm::mat4(1.0f));

	//roof
	ang = 45;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(ROOF, glm::vec3(28.0, 5.0, 28.0), glm::vec3(25, 9, -12), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROOF_ID);
	ang = -45;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(ROOF, glm::vec3(28, 5.0, 28.0), glm::vec3(25, 9, -35), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_ROOF_ID);

	//pillars
	createPillar(glm::vec3(18.5, 1, -27.2));
	createPillar(glm::vec3(27.6, 1, -27.2));
	createPillar(glm::vec3(18.5, 1, -19.8));
	createPillar(glm::vec3(27.6, 1, -19.8));

	addTavernItem(CRATE, glm::vec3(0.5, 1, 2), glm::vec3(21.9, 0.025, -13.5), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_BRANCHES_ID);
	addTavernItem(CRATE, glm::vec3(0.55, 0.1, 2), glm::vec3(21.9, 1.12, -13.5), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_MARBLE_ID);
	addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(21.9, 1.33, -13.5), glm::mat4(1.0f));
	addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(21.5, 1.33, -14), glm::mat4(1.0f));

	//door
	addTavernItem(DOOR, glm::vec3(1.5, 1.5, 1.5), glm::vec3(7.5, 1.35, -23), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 5;

	addTavernItem(BALUSTRADE, glm::vec3(4.5, 7.0, 4.0), glm::vec3(23.0, 8.0, -27.2), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 6;
	addTavernItem(BALUSTRADE, glm::vec3(4.5, 7.0, 4.0), glm::vec3(23.0, 8.0, -19.8), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 6;
	ang = 90;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(BALUSTRADE, glm::vec3(4.5, 7.0, 4.0), glm::vec3(17.9, 8.0, -23.5), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 6;
	addTavernItem(BALUSTRADE, glm::vec3(4.5, 7.0, 4.0), glm::vec3(28.2, 8.0, -23.5), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 6;
	// addTavernItem(BALUSTRADE, glm::vec3(4.5, 7.0, 4.0), glm::vec3(18.5, 8.0, -19.8), rot);

	//bar stools
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(11, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(12, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(12.9, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(13.5, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(15, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(16.2, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(17.5, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(18.2, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(18.9, .35, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	//side bar
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(23, .35, -15.4), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(23, .35, -14.2), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(23, .35, -13.6), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 10;

	createTable1(glm::vec3(11, 0, -25), 0);
	createTable1(glm::vec3(11, 0, -29), 0);
	createTable1(glm::vec3(11, 0, -33), 0);
	createTable1(glm::vec3(16, 0, -33), 0);
	createTable1(glm::vec3(20, 0, -33), 0);
	createTable1(glm::vec3(24, 0, -33), 0);
	createTable1(glm::vec3(28, 0, -33), 0);
	createTable1(glm::vec3(32.5, 0, -33), 0);
	createTable1(glm::vec3(36, 0, -33), 0);

	createTable1(glm::vec3(21.5, 0, -28), 0);
	createTable1(glm::vec3(25.5, 0, -28), 0);
	createTable1(glm::vec3(17, 0, -25), 90);
	createTable1(glm::vec3(30, 0, -26), 90);
	createTable1(glm::vec3(30, 0, -22), 90);

	createTable1(glm::vec3(37, 0, -27), 90);
	createTable1(glm::vec3(37, 0, -22), 90);
	createTable1(glm::vec3(37, 0, -18), 90);
	createTable1(glm::vec3(37, 0, -14), 90);

	createTable1(glm::vec3(34, 0, -14), 90);
	createTable1(glm::vec3(30, 0, -14), 90);
	createTable1(glm::vec3(26, 0, -14), 90);

	//vertical barrel
	addTavernItem(BARREL, glm::vec3(0.65, 0.85, 0.65), glm::vec3(10.0, 1.0, -13.0), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 11;
	//horizontal barrel
	ang = 90;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1.0f));
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(8.5, 0.68, -12.8), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 11;
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(8.5, 1.75, -13.6), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 11;
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(8.5, 0.68, -14.4), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 11;
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(8.5, 1.75, -15.2), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 11;
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(8.5, 0.68, -16), rot);
	tavernItems[tavernItems.size() - 1].materialNdx = 11;
	addTavernItem(CRATE, glm::vec3(0.95, 0.95, 0.95), glm::vec3(8.55, 1.0, -17.75), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_CRATE_ID);

	//adding bookshelves
	ang = 180;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(12.4, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(13.7, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(15, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(16.3, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(17.6, 1.75, -12.5), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_SHELF_ID);

	//counter
	ang = 90;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(CRATE, glm::vec3(0.5, 1, 5.0), glm::vec3(15, 0.025, -15), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_BRANCHES_ID);
	addTavernItem(CRATE, glm::vec3(0.55, 0.1, 5.0), glm::vec3(15, 1.12, -15), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_MARBLE_ID);
	addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(15, 1.33, -15), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 8;
	addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(17, 1.33, -15.3), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 8;
	addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(12, 1.33, -15.3), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 8;
	addTavernItem(BOTTLE, glm::vec3(.15, .15, .15), glm::vec3(19, 1.33, -15.3), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].materialNdx = 8;
	ang = 180;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(LANDLORD, glm::vec3(1, 1, 1), glm::vec3(15, 1.13, -13), rot);
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_LANDLORD_ID);
	addTavernItem(LUMBERJACK, glm::vec3(1, 1, 1), glm::vec3(15.5, .99, -16), glm::mat4(1.0f));
	tavernItems[tavernItems.size() - 1].loadTextureCoor(TAV_LUMBERJACK_ID);
	
	//fireplace and roasting
	addTavernCharacter(SAMURAI, glm::vec3(1, 1, 1), glm::vec3(20.05, 1.3, -23.5), glm::mat4(1.0f));
	tavernCharacters[tavernCharacters.size() - 1].meshes[0].loadTextureCoor(TAV_SAMURAI_ID);
	addTavernCharacter(SAMURAI, glm::vec3(1, 1, 1), glm::vec3(22.05, 1.3, -25.5), glm::mat4(1.0f));
	tavernCharacters[tavernCharacters.size() - 1].meshes[0].loadTextureCoor(TAV_SAMURAI_ID);
	addTavernCharacter(SAMURAI, glm::vec3(1, 1, 1), glm::vec3(21.05, 1.3, -22.5), glm::mat4(1.0f));
	tavernCharacters[tavernCharacters.size() - 1].meshes[0].loadTextureCoor(TAV_SAMURAI_ID);
	createFirePlace(glm::vec3(23.05, 1.5, -23.5));
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
	// printf("tried to load the texture %d\n", id);
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

void Tavern::drawTavern(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord, double ltime)
{
	applyTurkeySpin(ltime);
	for (int iter = 0; iter < tavernItems.size(); iter++) {
		//set a material
		if (tavernItems[iter].materialNdx != -1) {
			(*matSetter).setMaterial(tavernItems[iter].materialNdx);
			// printf("object %d update material to %d\n", iter, tavernItems[iter].materialNdx);
		}
		else {
			(*matSetter).setMaterial(9);
			// printf("material is now 9");
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
	
	for (int iter = 0; iter < tavernCharacters.size(); iter++) {
        for(int meshIter = 0; meshIter < tavernCharacters[iter].meshes.size(); meshIter++) {
        	if ((*fCuller).checkCull(tavernCharacters[iter].meshes[meshIter])) {
			    enableBuff(h_vertPos, h_vertNor, (*tavernCharacters[iter].meshes[meshIter].cont).posBuf, (*tavernCharacters[iter].meshes[meshIter].cont).norBuf, (*tavernCharacters[iter].meshes[meshIter].cont).indBuf);
			    if (tavernCharacters[iter].meshes[meshIter].hasTexture) {
			    	enableTextureBuffer(h_aTexCoord, tavernCharacters[iter].meshes[meshIter].texBuf, tavernCharacters[iter].meshes[meshIter].textureNdx);
			    }
			    tavernCharacters[iter].draw(h_ModelMatrix, meshIter);
			    disableBuff(h_vertPos, h_vertNor, h_aTexCoord);
			}
        }
	}
}

void Tavern::applyTurkeySpin(double ltime)
{
	float ang = 90;
	//rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	mat4 constRot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	curTime += (float)ltime / 100.0;
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
