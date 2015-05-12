#include "TerrainEvent.h"
#include <time.h>

#define SAMURAI 0
#define SPEARMAN 1
#define KNIGHT2 2
#define WARRIOR 3
#define STALL 4
#define MERCHANT 5
#define BOX 6
#define START_CITY 7
#define END_CITY 8
#define ROPE 9
#define BARREL 10
#define BOTTLE 11

#define NUM_TERR_EV_FILES 12

const string terrEvFiles[] = {"assets/events/samurai.obj",
							  "assets/events/spearman.obj",
							  "assets/events/knight2 split.obj",
							  "assets/events/warrior split.obj",
							  "assets/events/stall.obj",
							  "assets/events/merchant.obj",
							  "assets/events/box.obj",
							  "assets/events/startCity.obj",
							  "assets/events/endCity.obj",
							  "assets/events/rope.obj",
							  "assets/events/barrel.obj",
							  "assets/events/bottle.obj"
							 };

const vec3 objScales[] = {vec3(0.09, 0.09, 0.09),
		                  vec3(0.125, 0.125, 0.125),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(0.39, 0.45, 0.39),
		                  vec3(0.09, 0.09, 0.09),
		                  vec3(0.415, 0.04, 0.305),
						};

const float objYTrans[] = {0.085,
                           0.085,
                           1.0,
                           1.0,
                           0.22,
                           0.09,
                           0.40};

//characters rotate to face left of trail assuming right behind wagon
const mat4 objRotates[] = {glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)),
						   glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)),
						   mat4(1.0f),
						   mat4(1.0f),
						   glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)),
						   glm::rotate(mat4(1.0f), (const float)90, glm::vec3(1.0f, 0, 0)) * glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 0, 1.0f)),
						   glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)), //faces toward the end city
						};

// int TAV_CRATE_ID = 5000;
// int TAV_ROCK_ID = 6500;
int TERR_EV_STONE_ID = 7000;
int TERR_EV_START_CITY_ID = 7200;
int TERR_EV_MERCHANT_ID = 7300;
int TERR_EV_SPEARMAN_ID = 7400;
int TERR_EV_SAMURAI_ID = 7500;
int TERR_EV_CRATE_ID = 7700;
int TERR_EV_ROOF_ID = 7800;

// Obj3dContainer containers[std::extent<decltype(terrEvFiles)>::value];

int BRIDGE_NUM, ROPE_NUM;

//between [1, limit]
int TerrainEvent::getRandInt(int limit)
{
	return rand() % limit + 1;
}
//between [0, limit]
float TerrainEvent::getRandFloat(float limit)
{
	// return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / limit);/
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * limit * 2 - limit;//* spawnRad * 2 - spawnRad
}

TerrainEvent::TerrainEvent()
{

}

void TerrainEvent::init(Materials *newMatSetter, FrustumCull *newCuller)
{
	matSetter = newMatSetter;
	fCuller = newCuller;
	moveBridge = false;
	bridgeLoc = vec3(0, 0, 0);
	ropeLoc = vec3(0, 0, 0);
	bridgeAng = 110;
	ropeScale = 0;
}

void TerrainEvent::reset()
{
	moveBridge = false;
	bridgeLoc = vec3(0, 0, 0);
	ropeLoc = vec3(0, 0, 0);
	bridgeAng = 110;
	ropeScale = 0;

	eventItems.clear();
	eventCharacters.clear();
}

void TerrainEvent::addEventMesh(const string filename, bool noNorms)
{
	Obj3dContainer temp;
	temp.loadIntoTinyOBJ(filename);
	temp.initBuffers(noNorms);
	meshes.push_back(temp);
}

//index is the index of buffer info in tavernMeshes
void TerrainEvent::addEventItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
	Obj3d temp(&(meshes[index]), scale, trans, rot);
	eventItems.push_back(temp);
}

//index is the index of buffer info in tavernMeshes
void TerrainEvent::addEventCharacter(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
 //    vector<Obj3d> bodyParts;
	// Obj3d torso(&(tavernMeshes[index]), scale, trans, rot);
 //    // TODO our arm's translation should be based off rotation
	// Obj3d arm(&(tavernMeshes[CUBE]), glm::vec3(0.1, 0.5, 0.1), trans + glm::vec3(-0.3, 0.6, 0), rot);
 //    arm.preTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.4, 0.0));
 //    bodyParts.push_back(torso);
 //    bodyParts.push_back(arm);
    
	// tavernCharacters.push_back(*(new Mercenary(bodyParts)));
}

void TerrainEvent::loadTerrEvMeshes(TextureLoader* texLoader)
{
	srand(time(NULL));
	float ang;
	glm::mat4 rot;

	for (int iter = 0; iter < NUM_TERR_EV_FILES; iter++) {
		if (iter == SPEARMAN || iter == BARREL)
			addEventMesh(terrEvFiles[iter], true);
		else
			addEventMesh(terrEvFiles[iter], false);
	}

	//load textures
	texLoader->LoadTexture((char *)"assets/events/stone.bmp", TERR_EV_STONE_ID);
	texLoader->LoadTexture((char *)"assets/events/startCity.bmp", TERR_EV_START_CITY_ID);
	texLoader->LoadTexture((char *)"assets/events/merchant.bmp", TERR_EV_MERCHANT_ID);
	texLoader->LoadTexture((char *)"assets/events/spearman.bmp", TERR_EV_SPEARMAN_ID);
	texLoader->LoadTexture((char *)"assets/events/samuraiTex.bmp", TERR_EV_SAMURAI_ID); //yeah i know... its bad
	texLoader->LoadTexture((char *)"assets/events/crateTex.bmp", TERR_EV_CRATE_ID);
	texLoader->LoadTexture((char *)"assets/events/roof.bmp", TERR_EV_ROOF_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/logTex.bmp", TAV_LOG_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/lumberjackTex.bmp", TAV_LUMBERJACK_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/samuraiTex.bmp", TAV_SAMURAI_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/bookshelfTex.bmp", TAV_SHELF_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/marbletopTex.bmp", TAV_MARBLE_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/branchTex.bmp", TAV_BRANCHES_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/roofTex.bmp", TAV_ROOF_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/torchTex.bmp", TAV_TORCH_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/plankTex.bmp", TAV_PLANK_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/rockTex.bmp", TAV_ROCK_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/dirtTex.bmp", TAV_DIRT_ID);	
}

void TerrainEvent::addAmbush(vec3 loc, mat4 rot)
{
	float ambushLoc[] = {0,  0,
	                     0.09,  0.12,
	                     0.11, -0.16,
	                    -0.15,  0.05,
	                    -0.09, -0.17};

	int partySize = getRandInt(3) + 2;  //between 3 - 5 attackers
	for (int iter = 0; iter < partySize; iter++) {
		int num = getRandInt(2) - 1;
		vec3 trans = vec3(loc.x + ambushLoc[iter * 2], objYTrans[num], loc.z + ambushLoc[iter * 2 + 1]);
		mat4 newRot = rot * objRotates[num];
		addEventItem(num, objScales[num], trans, newRot);
		eventItems[eventItems.size() - 1].loadTextureCoor(findTex(num));
	}
}

void TerrainEvent::addMerchantStand(vec3 loc, mat4 rot)
{
	//trans to final pos * rot corresponding to rot variable * trans to certain dist out * scale

	mat4 newRot = rot * objRotates[STALL];
	addEventItem(STALL, objScales[STALL], vec3(loc.x, objYTrans[STALL], loc.z), newRot);
	eventItems[eventItems.size() - 1].materialNdx = 7;
	newRot = rot * objRotates[BOX];
	addEventItem(BOX, objScales[BOX], vec3(loc.x, objYTrans[BOX], loc.z), newRot);
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_ROOF_ID);
	newRot = rot * objRotates[MERCHANT];
	addEventItem(MERCHANT, objScales[MERCHANT], vec3(0, 0.23, 0), mat4(1.0f));
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_MERCHANT_ID);
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, objYTrans[MERCHANT], loc.z)) * newRot;

	addEventItem(BOX, vec3(0.05, 0.05, 0.05), vec3(0.295, 0, .09), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.05, loc.z)) * rot;
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_CRATE_ID);
	addEventItem(BOX, vec3(0.05, 0.05, 0.05), vec3(0.165, 0, .1), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.05, loc.z)) * rot;
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_CRATE_ID);
	addEventItem(BOX, vec3(0.05, 0.05, 0.05), vec3(0.225, 0, .098), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.15, loc.z)) * rot;
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_CRATE_ID);

	addEventItem(BOX, vec3(0.05, 0.05, 0.05), vec3(0.26, 0, -.32), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.05, loc.z)) * rot;
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_CRATE_ID);
	addEventItem(BARREL, vec3(0.055, 0.055, 0.055), vec3(0.09, 0, -.17), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.065, loc.z)) * rot;
	eventItems[eventItems.size() - 1].materialNdx = 11;
	addEventItem(BOTTLE, vec3(0.015, 0.015, 0.015), vec3(0.09, 0, -.17), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.1, loc.z)) * rot;
	eventItems[eventItems.size() - 1].materialNdx = 8;

	addEventItem(BARREL, vec3(0.055, 0.055, 0.055), vec3(-0.09, 0, .065), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.065, loc.z)) * rot;
	eventItems[eventItems.size() - 1].materialNdx = 11;
	addEventItem(BARREL, vec3(0.055, 0.055, 0.055), vec3(-0.2, 0, .08), mat4(1.0f));
	eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, 0.065, loc.z)) * rot;
	eventItems[eventItems.size() - 1].materialNdx = 11;


	float merchantLoc[] = {0.19, 0.3,
                           -0.35, 0.41,
                           -0.54, 0.3};

	int numBodyGuard = getRandInt(3) + 1; //between 1 - 3 bodyguards
	for (int iter = 0; iter < numBodyGuard; iter++) {
		int num = getRandInt(2) - 1;
		vec3 trans = vec3(merchantLoc[iter * 2], 0, merchantLoc[iter * 2 + 1]);
		mat4 newRot = rot * objRotates[num];
		addEventItem(num, objScales[num], trans, glm::mat4(1.0f));
		eventItems[eventItems.size() - 1].moveRot = glm::translate(mat4(1.0f), vec3(loc.x, objYTrans[num], loc.z)) * newRot;
		eventItems[eventItems.size() - 1].loadTextureCoor(findTex(num));
	}
}

void TerrainEvent::addRandomDuder(vec3 loc, mat4 rot) 
{
	int randDude = getRandInt(2) - 1;
	vec3 trans = vec3(loc.x, objYTrans[randDude], loc.z);
	mat4 newRot = rot * objRotates[randDude];
	addEventItem(randDude, objScales[randDude], trans, newRot);
	eventItems[eventItems.size() - 1].loadTextureCoor(findTex(randDude));
}

void TerrainEvent::addStartCity(vec3 loc)
{
	printf("Position : %f,%f,%f\n ",loc.x,loc.y,loc.z);
	addEventItem(START_CITY, vec3(1.65, 1.65, 1.65), vec3(loc.x, 0.685, loc.z), glm::rotate(mat4(1.0f), (const float)90, vec3(0, 1.0f, 0)));
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_START_CITY_ID);
}

void TerrainEvent::addEndCity(vec3 loc)
{
	addEventItem(END_CITY, vec3(4.05, 4.05, 4.05), vec3(loc.x, 0.45, loc.z - 0.2), glm::rotate(mat4(1.0f), (const float)-90, vec3(0, 1.0f, 0)));
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_STONE_ID);
	
	addEventItem(BOX, vec3(0.52, 0.05, 0.34), vec3(0.45, 0, 0), mat4(1.0f));
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_ROOF_ID);
	BRIDGE_NUM = eventItems.size() - 1;
	mat4 bridgeRot = glm::rotate(mat4(1.0f), bridgeAng, vec3(0, 0, 1.0f));
	bridgeLoc = vec3(loc.x - 1.9, 0, loc.z - 1.065);
	eventItems[BRIDGE_NUM].moveRot = glm::translate(mat4(1.0f), bridgeLoc) * bridgeRot;
	
	ropeScale = 0.1;
	mat4 ropeRot = glm::rotate(mat4(1.0f), (const float)-90, vec3(0, 1.0f, 0)) * glm::rotate(mat4(1.0f), (const float)45, vec3(1.0f, 0, 0));
	ropeLoc = vec3(loc.x - 1.92, 0.9, loc.z - 1.32);
	addEventItem(ROPE, vec3(0.15, 0.15, ropeScale), ropeLoc, ropeRot);
	ROPE_NUM = eventItems.size() - 1;

	addEventItem(ROPE, vec3(0.15, 0.15, ropeScale), vec3(ropeLoc.x, ropeLoc.y, ropeLoc.z + 0.5), ropeRot);

	addEventItem(BOX, vec3(0.15, 0.15, 0.42), vec3(loc.x - 1.9, 0.84, loc.z - 1.065), mat4(1.0f));
	eventItems[eventItems.size() - 1].loadTextureCoor(TERR_EV_STONE_ID);
}

int TerrainEvent::findTex(int num) 
{
	int found = -1;
	switch (num) {
		case 0:
			found = TERR_EV_SAMURAI_ID;
			break;
		case 1:
			found = TERR_EV_SPEARMAN_ID;
			break;
	}
	return found;
}

void TerrainEvent::lowerBridge()
{
	if (bridgeAng < 180) {
		moveBridge = true;
	}
}

void TerrainEvent::enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf) {
  GLSL::enableVertexAttribArray(h_vertPos); //position
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLSL::enableVertexAttribArray(h_vertNor); //normals
  glBindBuffer(GL_ARRAY_BUFFER, norBuf);
  glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
}

void TerrainEvent::disableBuff(GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord) {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_vertNor);
  GLSL::disableVertexAttribArray(h_aTexCoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisable(GL_TEXTURE_2D);
}

void TerrainEvent::enableTextureBuffer(GLint h_aTexCoord, GLuint texBuf, int id)
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

void TerrainEvent::setBridge(double ltime)
{
	//trans to final pos in world * moving angle * trans up a bit * rot to face right dir * scale
	// printf("ltime is %f and bridge ang is %f\n", ltime, bridgeAng);

	// float timeInc = (float)ltime * 10;	//for those random times my computer speeds up -_-
	// bridgeAng += (timeInc > 0.25) ? 0.25 : timeInc;
	bridgeAng += 0.25;
	mat4 bridgeRot = glm::rotate(mat4(1.0f), bridgeAng, vec3(0, 0, 1.0f));
	eventItems[BRIDGE_NUM].moveRot = glm::translate(mat4(1.0f), bridgeLoc) * bridgeRot;

	// ropeScale += ltime / 12.0;
	// timeInc = ltime / 12.0;	//for those random times my computer speeds up -_-
	// ropeScale += (timeInc > 0.00165) ? 0.00165 : timeInc;
	ropeScale += 0.00165;
	eventItems[ROPE_NUM].scale = vec3(0.15, 0.15, ropeScale);
	eventItems[ROPE_NUM + 1].scale = vec3(0.15, 0.15, ropeScale);
	// ropeLoc = vec3(ropeLoc.x - .001, ropeLoc.y -.001, ropeLoc.z);
	ropeLoc = vec3(ropeLoc.x - .00175, ropeLoc.y -.00175, ropeLoc.z);
	eventItems[ROPE_NUM].pos = ropeLoc;
	eventItems[ROPE_NUM + 1].pos = vec3(ropeLoc.x, ropeLoc.y, ropeLoc.z + 0.5);
	if (bridgeAng > 180) {
		moveBridge = false;
	}
}

void TerrainEvent::drawTerrainEvents(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord, double ltime)
{
	if (moveBridge) {
		setBridge(ltime);
	}
	for (int iter = 0; iter < eventItems.size(); iter++) {
		//set a material

		if (eventItems[iter].materialNdx != -1) {
			(*matSetter).setMaterial(eventItems[iter].materialNdx);
			// printf("object %d update material to %d\n", iter, tavernItems[iter].materialNdx);
		}
		else {
			(*matSetter).setMaterial(9);
			// printf("material is now 9");
		}
		enableBuff(h_vertPos, h_vertNor, (*eventItems[iter].cont).posBuf, (*eventItems[iter].cont).norBuf, (*eventItems[iter].cont).indBuf);
		if (eventItems[iter].hasTexture) {
			enableTextureBuffer(h_aTexCoord, eventItems[iter].texBuf, eventItems[iter].textureNdx);
		}
		//decide whether to cull
		// if ((*fCuller).checkCull(eventItems[iter])) {
			eventItems[iter].draw(h_ModelMatrix);
		// }
		disableBuff(h_vertPos, h_vertNor, h_aTexCoord);
	}
	
	// for (int iter = 0; iter < tavernCharacters.size(); iter++) {
 //        for(int meshIter = 0; meshIter < tavernCharacters[iter].meshes.size(); meshIter++) {
 //        	if ((*fCuller).checkCull(tavernCharacters[iter].meshes[meshIter])) {
	// 		    enableBuff(h_vertPos, h_vertNor, (*tavernCharacters[iter].meshes[meshIter].cont).posBuf, (*tavernCharacters[iter].meshes[meshIter].cont).norBuf, (*tavernCharacters[iter].meshes[meshIter].cont).indBuf);
	// 		    if (tavernCharacters[iter].meshes[meshIter].hasTexture) {
	// 		    	enableTextureBuffer(h_aTexCoord, tavernCharacters[iter].meshes[meshIter].texBuf, tavernCharacters[iter].meshes[meshIter].textureNdx);
	// 		    }
	// 		    tavernCharacters[iter].draw(h_ModelMatrix, meshIter);
	// 		    disableBuff(h_vertPos, h_vertNor, h_aTexCoord);
	// 		}
 //        }
	// }
}