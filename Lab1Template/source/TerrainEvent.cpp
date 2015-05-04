#include "TerrainEvent.h"
#include <time.h>

#define WEREWOLF 0
#define KNIGHT 1
#define KNIGHT2 2
#define WARRIOR 3
#define CART 4
#define HORSE 5

#define NUM_TERR_EV_FILES 0

const string terrEvFiles[] = {"assets/events/werewolf.obj",
							  "assets/events/knight.obj",
							  "assets/events/knight2.obj",
							  "assets/events/warrior.obj",
							  "assets/events/cart.obj",
							  "assets/events/horse.obj"
							 };

const vec3 objScales[] = {vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
		                  vec3(1.0, 1.0, 1.0),
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
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / limit);
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

void TerrainEvent::loadTavernMeshes(TextureLoader* texLoader)
{
	srand(time(NULL));
	float ang;
	glm::mat4 rot;

	for (int iter = 0; iter < NUM_TERR_EV_FILES; iter++) {
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
	int locations[] = {0,  0,
	                   1,  1,
	                   1, -1,
	                  -1,  1,
	                  -1, -1};

	int partySize = getRandInt(3) + 2;
	
	for (int iter = 0; iter < partySize; iter++) {
		int num = getRandInt(4) - 1;
		int xLoc = getRandFloat(1.0);
		int zLoc = getRandFloat(1.0);
		vec3 trans = vec3(loc.x + xLoc * locations[iter * 2], loc.y, loc.z + zLoc * locations[iter * 2 + 1]);
		addEventItem(num, vec3(1.0, 1.0, 1.0), trans, rot);
	}
}

void TerrainEvent::addMerchantStand(vec3 loc, mat4 rot)
{
	addEventItem(CART, vec3(1.0, 1.0, 1.0), loc, rot);
	addEventItem(HORSE, vec3(1.0, 1.0, 1.0), vec3(loc.x + 1.0, loc.y, loc.z), rot);
	//addEventItem(MERCHANT, vec3(1.0, 1.0, 1.0), loc, rot);
	int numBodyGuard = getRandInt(2) + 1;
	for (int iter = 0; iter < numBodyGuard; iter++) {
		// int num = getRandInt(4) - 1;
		// int xLoc = getRandFloat(1.0);
		// int zLoc = getRandFloat(1.0);
		// vec3 trans = vec3(loc.x + xLoc * locations[iter * 2], loc.y, loc.z + zLoc * locations[iter * 2 + 1]);
		// addEventItem(num, vec3(1.0, 1.0, 1.0), trans, rot);
	}
}

void TerrainEvent::addRandomDuder(vec3 loc, mat4 rot) 
{
	int randDude = getRandInt(3) + 1;
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
		if ((*fCuller).checkCull(eventItems[iter])) {
			eventItems[iter].draw(h_ModelMatrix);
		}
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