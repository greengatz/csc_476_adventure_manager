#include "TerrainEvent.h"
#include <time.h>

#define SAMURAI 0
#define SPEARMAN 1
#define KNIGHT2 2
#define WARRIOR 3
#define STALL 4
#define ROOF 5

#define NUM_TERR_EV_FILES 6

const string terrEvFiles[] = {"assets/events/samurai.obj",
							  "assets/events/spearman.obj",
							  "assets/events/knight2.obj",
							  "assets/events/warrior.obj",
							  "assets/events/stall.obj",
							  "assets/events/box.obj"
							 };

const vec3 objScales[] = {vec3(0.09, 0.09, 0.09),
		                  vec3(0.125, 0.125, 0.125),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(0.39, 0.45, 0.39),
		                  vec3(0.415, 0.04, 0.305),
						};

const float objYTrans[] = {0.085,
                           0.085,
                           1.0,
                           1.0,
                           0.22,
                           0.40};

//characters rotate to face left of trail assuming right behind wagon
const mat4 objRotates[] = {glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)),
						   glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)),
						   mat4(1.0f),
						   mat4(1.0f),
						   glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)),
						   glm::rotate(mat4(1.0f), (const float)180, glm::vec3(0, 1.0f, 0)), //faces toward the end city
						};

// int TAV_CRATE_ID = 5000;
// int TAV_LANDLORD_ID = 5500;
// int TAV_LOG_ID = 5600;
// int TAV_LUMBERJACK_ID = 5700;
// int TAV_SAMURAI_ID = 5800;
// int TAV_SHELF_ID = 5900;
// int TAV_MARBLE_ID = 6000;
// int TAV_BRANCHES_ID = 6100;
// int TAV_ROOF_ID = 6200;
// int TAV_TORCH_ID = 6300;
// int TAV_PLANK_ID = 6400;
// int TAV_ROCK_ID = 6500;
// int TAV_DIRT_ID = 6600;

// Obj3dContainer containers[std::extent<decltype(terrEvFiles)>::value];

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
		if (iter == SPEARMAN)
			addEventMesh(terrEvFiles[iter], true);
		else
			addEventMesh(terrEvFiles[iter], false);
	}

	//load textures
	// texLoader->LoadTexture((char *)"assets/tavern/crateTex.bmp", TAV_CRATE_ID);
	// texLoader->LoadTexture((char *)"assets/tavern/landlordTex.bmp", TAV_LANDLORD_ID);
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
	}
}

void TerrainEvent::addMerchantStand(vec3 loc, mat4 rot)
{
	mat4 newRot = rot * objRotates[STALL];
	addEventItem(STALL, objScales[STALL], vec3(loc.x, objYTrans[STALL], loc.z), newRot);
	newRot = rot * objRotates[ROOF];
	addEventItem(ROOF, objScales[ROOF], vec3(loc.x, objYTrans[ROOF], loc.z), newRot);
	//addEventItem(MERCHANT, vec3(1.0, 1.0, 1.0), loc, rot);

	float merchantLoc[] = {-0.19, -0.3,
                           0.35, -0.41,
                           0.54, -0.3};

	int numBodyGuard = getRandInt(3) + 1; //between 1 - 3 bodyguards
	for (int iter = 0; iter < numBodyGuard; iter++) {
		int num = getRandInt(2) - 1;
		vec3 trans = vec3(loc.x + merchantLoc[iter * 2], objYTrans[num], loc.z + merchantLoc[iter * 2 + 1]);
		mat4 newRot = rot * objRotates[num];
		addEventItem(num, objScales[num], trans, newRot);
	}
}

void TerrainEvent::addRandomDuder(vec3 loc, mat4 rot) 
{
	int randDude = getRandInt(2) - 1;
	vec3 trans = vec3(loc.x, objYTrans[randDude], loc.z);
	mat4 newRot = rot * objRotates[randDude];
	addEventItem(randDude, objScales[randDude], trans, newRot);
	addEventItem(randDude, vec3(1.0, 1.0, 1.0), loc, rot);
}

void TerrainEvent::startCity(vec3 loc)
{

}

void TerrainEvent::endCity(vec3 loc)
{
	
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

void TerrainEvent::drawTerrainEvents(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord)
{
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