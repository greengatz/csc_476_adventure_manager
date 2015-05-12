/**
 * Terrain.cpp - Terrain object
 * @author Brandon Clark
 */

#include <cstdlib>
#include <time.h> 
#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "Terrain.h"
#include "GLSL.h"
#include "splineCurve.h"

#define GRASS 3
#define TRAIL 0
#define RED 1
#define LBTRAIL 2
#define RBTRAIL 4
#define LTTRAIL 5
#define RTTRAIL 6
#define LNEARTRAIL 7
#define RNEARTRAIL 8
#define LFARTRAIL 9
#define RFARTRAIL 10

//This probably isnt a good idea....
using namespace std;

int TERRAIN_TEX_ID = 100;
int TERRAIN_TEX_DIRT_ID = 200;
int TERRAIN_TEX_RED_ID = 300;
int TERRAIN_TEX_LBTRAIL_ID = 400;
int TERRAIN_TEX_RBTRAIL_ID = 900;
int TERRAIN_TEX_LTTRAIL_ID = 500;
int TERRAIN_TEX_RTTRAIL_ID = 800;
int TERRAIN_TEX_LNEARTRAIL_ID = 600;
int TERRAIN_TEX_RNEARTRAIL_ID = 1000;
int TERRAIN_TEX_LFARTRAIL_ID = 1100;
int TERRAIN_TEX_RFARTRAIL_ID = 1200;

Terrain::Terrain() :
	x(0.0f, 0.0f, 0.0f),
	scale(5.0f),
	posBufID(0),
	norBufID(0),
	texBufID(0),
  oldTextureID(0),
  beginPosition(0.0f, 0.0f, 0.0f),
  criticalPoints(std::vector<glm::vec3>()),
  terrainEvents()
{
}

Terrain::~Terrain()
{
}

glm::vec3 Terrain::getStartPosition()
{
   return criticalPoints[0];
}

void Terrain::printCriticalPoints()
{
  for (std::vector<glm::vec3>::iterator itr = criticalPoints.begin(); 
      itr != criticalPoints.end(); itr++)
   {
        printf("Critical point at %f,%f,%f\n", itr->x, itr->y, itr->z);
   }
}

glm::vec3 Terrain::nextCriticalPoint(glm::vec3 aPos)
{
   nextCPoint++;
   return criticalPoints[nextCPoint - 1];
}

bool Terrain::atEnd(glm::vec3 aPos)
{
   if (aPos.x >= ((MAP_Z - 1) * MAP_SCALE) - 0.5)
   {
      return true;
   }
   return false;
}

int Terrain::checkEvents(glm::vec3 aPos){
    int event = 0;
    int spot = floor(aPos.x);
    if(spot > MAP_Z - 10){
      terrainEvents.lowerBridge();
    }
    if(eventsMap[spot] == MERCHANT){
        printf("%s\n", "You stumbled upon a merchant.");
        event = MERCHANT;
    }
    if(eventsMap[spot] == AMBUSH){
        printf("%s\n", "Bandits are ambushing your party!");
        event = AMBUSH;
    }
    if(eventsMap[spot] == WANDERER){
        printf("%s\n", "A lone wanderer joins your party.");
        event = WANDERER;
    }
    if(eventsMap[spot] == SICKNESS){
        printf("%s\n", "One of you troops just caught the plague!");
        event = SICKNESS;
    }
    eventsMap[spot] = 0;
    return event;
}

void Terrain::placeEvents(){
  for(int i = 0; i < MAP_Z; i++){
    glm::vec3 temp = criticalPoints[i];
    if(eventsMap[i] == AMBUSH){
      temp.x -= 100;
      temp.z -= 1;
      terrainEvents.addAmbush(temp, glm::mat4(1.0f));
      printf("Placing ambush at %f, %f\n", temp.x, temp.z);

    }
    if(eventsMap[i] == MERCHANT){
      temp.x -= 100;
      temp.z += 1.5;
      terrainEvents.addMerchantStand(temp, glm::mat4(1.0f));
      printf("Placing merchant at %f, %f\n", temp.x, temp.z);
    }
    if(eventsMap[i] == WANDERER){
      temp.x -= 100;
      // temp.z += 0.5;
      terrainEvents.addRandomDuder(temp, glm::mat4(1.0f));
      printf("Placing merchant at %f, %f\n", temp.x, temp.z);
    }
  }
}

void Terrain::createEvents(){
    int minMerch = 1, maxMerch = 2,      //Min max chance of Merchant event
        minWand = 1, maxWand = 2,        //Min max chance of Random Wanderer event
        minAmbush = 1, maxAmbush = 3,    //Min max chance of Ambush event
        minSick = 2, maxSick = 5;        //Min max chance of Sickness event 
    int startingOffset = 3;      //No events from starting 3 spaces
    int endingOffset = 5;      //No events from ending 5 spaces

    for(int i = 0; i < MAP_X; i++){
        eventsMap[i] = 0;
    }
    srand(time(NULL));

    //Place Merchant
    int merchCount = (rand() % (maxMerch - minMerch)) + minMerch;
    for(int i = 0; i <= merchCount; i++){
        int random = ((rand() % (MAP_X - startingOffset - endingOffset)) +  startingOffset);
        if(eventsMap[random] == 0 && eventsMap[random + 1] == 0 && eventsMap[random - 1] == 0)
            eventsMap[random] = MERCHANT; 
        else
            merchCount++;
    }

    //Place Wanderer
    int wandCount = (rand() % (maxWand - minWand)) + minWand;
    for(int i = 0; i <= wandCount; i++){
        int random = ((rand() % (MAP_X - startingOffset - endingOffset)) +  startingOffset);
        if(eventsMap[random] == 0 && eventsMap[random + 1] == 0 && eventsMap[random - 1] == 0)
            eventsMap[random] = WANDERER; 
        else
            wandCount++;
    }

    //Place Ambush
    int ambushCount = (rand() % (maxAmbush - minAmbush)) + minAmbush;
    for(int i = 0; i <= ambushCount; i++){
        int random = ((rand() % (MAP_X - startingOffset - endingOffset)) +  startingOffset);
        if(eventsMap[random] == 0 && eventsMap[random + 1] == 0 && eventsMap[random - 1] == 0){
            eventsMap[random] = AMBUSH; 
            // printf("Ambush on trail at : %f", (float)random); 
        }
        else
            ambushCount++;
    }

    //Place Sickness
    int sickCount = (rand() % (maxSick - minSick)) + minSick;
    for(int i = 0; i <= sickCount ; i++){
        int random = ((rand() % (MAP_X - startingOffset)) +  startingOffset);
        if(eventsMap[random] == 0 && eventsMap[random + 1] == 0 && eventsMap[random - 1] == 0)
            eventsMap[random] = SICKNESS; 
        else
            sickCount++;
    }
    printf("MerchCount:%d, Wanderer:%d, Ambush:%d, Sickness:%d\n", merchCount, wandCount, ambushCount, sickCount);
    for(int i = 0; i <= MAP_Z ; i++){
        printf("[%d]", eventsMap[i]);
    }

    placeEvents();
}

// i fixed one of 3 problem spots
void Terrain::createTrail(){
    criticalPoints.clear();

    srand(time(NULL));
    int minShift = 6, maxShift = 12;
    //left for false, right for true.
    bool shiftTog = (rand() % 2) == 0 ? false : true; 
    int indexX, indexZ, bound = 20;
    startingSpot = ((rand() % (MAP_X - bound)) + (bound / 2));
    int lastSpot = startingSpot;
    int changeInPath = (rand() % (maxShift - minShift)) + minShift;
    vector<glm::vec2> splineSeed;
    int direction = 1;
    nextCPoint = 1;


    splineSeed.clear();
    // determine our critical points to make the spline
    splineSeed.push_back(glm::vec2(0, -25)); // add first point to spline
    // x is our depth through the trail
    // x = 50 is end
    for (indexZ = 0; indexZ < MAP_Z; indexZ++) {
        if(changeInPath == 0) {
            changeInPath = (rand() % (maxShift - minShift)) + minShift;
            int newZ = (rand() % (maxShift - minShift)) * direction + splineSeed[splineSeed.size() - 1].y;
            splineSeed.push_back(glm::vec2(indexZ, newZ));
            cout << splineSeed[splineSeed.size() - 1].x << ", " << splineSeed[splineSeed.size() - 1].y << "\n";
        }
        else {//if(changeInPath == 1){
            changeInPath--;
            direction = -direction;
        }
    }
    
    splineSeed.push_back(glm::vec2(50, splineSeed[splineSeed.size() - 1].y)); // add last point to spline
    path = new Spline(splineSeed, 0, 0);

    for(int i = 0; i < 50; i++) { // reverse these
        cout << "cp: " << i << ", " << path->getY(i) << "\n";
        criticalPoints.push_back(glm::vec3(i, 0, path->getY(i)));
        printf("criticalPoints[%d]:%f,%f,%f \n", i, criticalPoints[i].x,criticalPoints[i].y,criticalPoints[i].z);
    }

    // using the spline, draw generate a path
    // everything is grass to start
    for (indexZ = 0; indexZ < MAP_Z; indexZ++){
        for (indexX = 0; indexX < MAP_X; indexX++) {
            trailMap[indexX][indexZ] = GRASS;
        }
    }

    // draw the main trail
    for (indexZ = 0; indexZ < MAP_Z; indexZ++){
        for (indexX = 0; indexX < MAP_X; indexX++) {
            if(abs(indexZ + path->getY(indexX) + 0.5) < 1) {
                trailMap[indexZ][indexX]=TRAIL;
              
            }
        }
    }

    // draw the sides of the trail

    // 4 diagonal trail sides
    for(indexZ = 0; indexZ < MAP_Z - 1; indexZ++) {
        for(indexX = 0; indexX < MAP_X; indexX++) {
            if(trailMap[indexX][indexZ] == TRAIL
                && trailMap[indexX - 1][indexZ + 1] == TRAIL
                && trailMap[indexX - 1][indexZ] == GRASS) {
                trailMap[indexX - 1][indexZ] = LBTRAIL;
            }
        }
    }
    
    for(indexZ = 0; indexZ < MAP_Z - 1; indexZ++) {
        for(indexX = 0; indexX < MAP_X; indexX++) {
            if(trailMap[indexX][indexZ] == TRAIL
                && trailMap[indexX + 1][indexZ + 1] == TRAIL
                && trailMap[indexX + 1][indexZ] == GRASS) {
                trailMap[indexX + 1][indexZ] = RTTRAIL;
            }
        }
    }
    
    for(indexZ = 0; indexZ < MAP_Z; indexZ++) {
        for(indexX = 0; indexX < MAP_X - 1; indexX++) {
            if(trailMap[indexX][indexZ] == TRAIL
                && trailMap[indexX - 1][indexZ - 1] == TRAIL
                && trailMap[indexX - 1][indexZ] == GRASS) {
                trailMap[indexX - 1][indexZ] = RBTRAIL;
            }
        }
    }
    
    for(indexZ = 0; indexZ < MAP_Z; indexZ++) {
        for(indexX = 1; indexX < MAP_X - 1; indexX++) {
            if(trailMap[indexX][indexZ] == TRAIL
                && trailMap[indexX + 1][indexZ - 1] == TRAIL
                && trailMap[indexX + 1][indexZ] == GRASS) {
                trailMap[indexX + 1][indexZ ] = LTTRAIL;
            }
        }
    }

/*#define LNEARTRAIL 7
#define RNEARTRAIL 8
#define LFARTRAIL 9
#define RFARTRAIL 10*/
    // flat trail sides
    for(indexZ = 0; indexZ < MAP_Z; indexZ++) {
        for(indexX = 0; indexX < MAP_X; indexX++) {
            if(trailMap[indexX][indexZ] == TRAIL
                && trailMap[indexX + 1][indexZ] == GRASS) {
                trailMap[indexX][indexZ] = RFARTRAIL;
            }
        }
    }
    
    for(indexZ = 0; indexZ < MAP_Z; indexZ++) {
        for(indexX = 0; indexX < MAP_X; indexX++) {
            if(trailMap[indexX][indexZ] == TRAIL
                && trailMap[indexX - 1][indexZ] == GRASS) {
                trailMap[indexX][indexZ] = LNEARTRAIL;
            }

            //Stuffed in here to avoid writing another for loop
            randomTree[indexX][indexZ] = 1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(3.0-1.0)));
        }
    }
    
    terrainEvents.reset();
    
    vec3 temp = criticalPoints[0];
    temp.x -= 101;
    temp.y += 0.1;
    terrainEvents.addStartCity(temp);

    vec3 temp2 = criticalPoints[MAP_X - 1];

    temp2.x -= 100;
    temp2.y -= 0.1;
    temp2.z += 1;
    // temp2.z = temp.z * -1.0;
    terrainEvents.addEndCity(temp2);
        
    startingSpot = criticalPoints[0].z;
    beginPosition = criticalPoints[0];

    if (shiftTog)
      criticalPoints.push_back(glm::vec3((float)((MAP_X+1.0) * MAP_SCALE), 0.0, -lastSpot - 1.0));
    else
      criticalPoints.push_back(glm::vec3((float)((MAP_X+1.0) * MAP_SCALE), 0.0, -lastSpot));
    
    path->printSpline();
    createEvents();
}

void Terrain::init(TextureLoader* texLoader, Materials *matSetter, FrustumCull *fCuller)
{
  terrainEvents.init(matSetter, fCuller);
  terrainEvents.loadTerrEvMeshes(texLoader);
	x.x = 0.0f;
	x.y = 0.0f;
	x.z = 0.0f;
	scale = 5.0f;
  createTrail();
	GLfloat terrain_buffer[(MAP_Z - 1) * (MAP_X - 1) * 12];
  GLfloat terrain_norm[(MAP_Z - 1) * (MAP_X - 1) * 12];
  GLfloat terrain_tex[(MAP_Z - 1) * (MAP_X - 1) * 8];

  	int index = 0;
  	// loop through all of the heightfield points, calculating
  	// the coordinates for each point
  	for (int z = 0; z < MAP_Z; z++)
  	{
    	for (int x = 0; x < MAP_X; x++, index++)
    	{
      	terrain[x][z][0] = float(x) * MAP_SCALE;    
      	terrain[x][z][1] = 0; //restrict y to 0
      	terrain[x][z][2] = -float(z) * MAP_SCALE;
    	}
  	}

  	//initialize normals to zero
  	for (int x = 0; x < (MAP_Z - 1) * (MAP_X - 1) * 12; x++)
  	{
    	terrain_buffer[x] = 0;
    	terrain_norm[x] = 0;
  	}

  	index = 0;
  	for (int z = 0; z < MAP_Z - 1; z++)
  	{	
    	for (int x = 0; x < MAP_X - 1; x++, index++)
    	{

      	//Calculate normal
      	glm::vec3 v1 = glm::vec3(terrain[x][z][0], terrain[x][z][1], terrain[x][z][2]);
      	glm::vec3 v2 = glm::vec3(terrain[x+1][z][0], terrain[x+1][z][1], terrain[x+1][z][2]);
      	glm::vec3 v3 = glm::vec3(terrain[x][z+1][0], terrain[x][z+1][1], terrain[x][z+1][2]);
      	glm::vec3 nor = glm::cross(v2-v1, v3-v1);
      
      	//v1
      	terrain_buffer[12 * index + 0] = terrain[x][z][0]; //x
      	terrain_buffer[12 * index + 1] = terrain[x][z][1]; //y
      	terrain_buffer[12 * index + 2] = terrain[x][z][2]; //z
      	//add normal to sum
      	terrain_norm[12 * index + 0] += nor.x;
      	terrain_norm[12 * index + 1] += nor.y;
      	terrain_norm[12 * index + 2] += nor.z;
      	//set texture coordinate for that point
      	terrain_tex[8 * index + 0] = 0.0;
      	terrain_tex[8 * index + 1] = 0.0;

      	//v2
      	terrain_buffer[12 * index + 3] = terrain[x+1][z][0];
      	terrain_buffer[12 * index + 4] = terrain[x+1][z][1];
      	terrain_buffer[12 * index + 5] = terrain[x+1][z][2];
      	terrain_norm[12 * index + 3] += nor.x;
      	terrain_norm[12 * index + 4] += nor.y;
      	terrain_norm[12 * index + 5] += nor.z;
      	terrain_tex[8 * index + 2] = 1.0;
      	terrain_tex[8 * index + 3] = 0.0;

      	//v3
      	terrain_buffer[12 * index + 6] = terrain[x][z+1][0];
      	terrain_buffer[12 * index + 7] = terrain[x][z+1][1];
      	terrain_buffer[12 * index + 8] = terrain[x][z+1][2];
      	terrain_norm[12 * index + 6] += nor.x;
      	terrain_norm[12 * index + 7] += nor.y;
      	terrain_norm[12 * index + 8] += nor.z;
      	terrain_tex[8 * index + 4] = 0.0;
      	terrain_tex[8 * index + 5] = 1.0;

      	//v4
      	terrain_buffer[12 * index + 9] = terrain[x+1][z+1][0];
      	terrain_buffer[12 * index + 10] = terrain[x+1][z+1][1];
      	terrain_buffer[12 * index + 11] = terrain[x+1][z+1][2];
      	terrain_norm[12 * index + 9] += nor.x;
      	terrain_norm[12 * index + 10] += nor.y;
      	terrain_norm[12 * index + 11] += nor.z;
      	terrain_tex[8 * index + 6] = 1.0;
      	terrain_tex[8 * index + 7] = 1.0;
    	}
  	}

  	//normalize the normals
  	for (int x = 0; x < ((MAP_Z - 1) * (MAP_X - 1) * 12)/3; x += 3)
  	{
    	glm::vec3 nor = glm::normalize(glm::vec3(terrain_norm[x], terrain_norm[x+1], 
      terrain_norm[x+2]));

    	terrain_norm[x] = nor.x;
    	terrain_norm[x+1] = nor.y;
    	terrain_norm[x+2] = nor.z;
  	}

  	glGenBuffers(1, &posBufID);
  	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_buffer), terrain_buffer, GL_STATIC_DRAW);
  
  	glGenBuffers(1, &norBufID);
  	glBindBuffer(GL_ARRAY_BUFFER, norBufID);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_norm), terrain_norm, GL_STATIC_DRAW);
  
  	glGenBuffers(1, &texBufID);
  	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_tex), terrain_tex, GL_STATIC_DRAW);

    //Load Texture
    texLoader->LoadTexture((char *)"assets/grass2.bmp", TERRAIN_TEX_ID);
    texLoader->LoadTexture((char *)"assets/trail2.bmp", TERRAIN_TEX_DIRT_ID);
    texLoader->LoadTexture((char *)"assets/trailtopleft.bmp", TERRAIN_TEX_LBTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/trailbottomleft.bmp", TERRAIN_TEX_RBTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/trailbottomright.bmp", TERRAIN_TEX_RTTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/trailtopright.bmp", TERRAIN_TEX_LTTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/red.bmp", TERRAIN_TEX_RED_ID);
    texLoader->LoadTexture((char *)"assets/trailfarleft.bmp", TERRAIN_TEX_LFARTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/trailfarright.bmp", TERRAIN_TEX_RFARTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/trailnearleft.bmp", TERRAIN_TEX_LNEARTRAIL_ID);
    texLoader->LoadTexture((char *)"assets/trailnearright.bmp", TERRAIN_TEX_RNEARTRAIL_ID);

  	//unbind the arrays
  	glBindBuffer(GL_ARRAY_BUFFER, 0);

   tree.init(texLoader);
   
	assert(glGetError() == GL_NO_ERROR);
}


void Terrain::draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, Camera* camera, glm::vec3 wagonPos, GLuint* pid)
{
  //set up the texture unit
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);

	// Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_nor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufID);
   glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_pos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, texBufID);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   int size = 0;
   for (int index = 0; index < MAP_X - 1; index++)
   {
        for(int index2 = 0; index2 < MAP_Z - 1; index2++){
            if(trailMap[index][index2] == RED){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_RED_ID);
            }else if(trailMap[index][index2] == GRASS){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_ID);
            }else if(trailMap[index][index2] == TRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_DIRT_ID);
            }else if(trailMap[index][index2] == LBTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_LBTRAIL_ID);
            }else if(trailMap[index][index2] == RBTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_RBTRAIL_ID);
            }else if(trailMap[index][index2] == RTTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_RTTRAIL_ID);
            }else if(trailMap[index][index2] == LTTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_LTTRAIL_ID);
            }else if(trailMap[index][index2] == RFARTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_RFARTRAIL_ID);
            }else if(trailMap[index][index2] == LFARTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_LFARTRAIL_ID);
            }else if(trailMap[index][index2] == RNEARTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_RNEARTRAIL_ID);
            }else if(trailMap[index][index2] == LNEARTRAIL){
                glBindTexture(GL_TEXTURE_2D, TERRAIN_TEX_LNEARTRAIL_ID);
            }

            if (trailMap[index][index2] != oldTextureID)
            {
               oldTextureID = trailMap[index][index2];
            }

            glDrawArrays(GL_TRIANGLE_STRIP, size, 4);
            size +=  4;
        }
   }

   GLSL::disableVertexAttribArray(h_pos);
   GLSL::disableVertexAttribArray(h_nor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, 0); 
   glDisable(GL_TEXTURE_2D);

   terrainEvents.drawTerrainEvents(h_ModelMatrix, h_pos, h_nor, h_aTexCoord, 0.0);
   for (int index = 0; index < MAP_X - 1; index++)
   {
      for(int index2 = 0; index2 < MAP_Z - 3; index2++)
      {
         if(trailMap[index][index2] == LBTRAIL || trailMap[index][index2] == RBTRAIL
            || trailMap[index][index2] == LFARTRAIL || trailMap[index][index2] == LNEARTRAIL)
         {
            tree.draw(glm::vec3(index2, 0.0, -index + startingSpot + randomTree[index][index2]), camera, wagonPos);
            tree.draw(glm::vec3(index2 + 0.5, 0.0, -index + startingSpot + randomTree[index][index2] + 2.0), camera, wagonPos);
            tree.drawBillboard(glm::vec3(index2, 0.0, -index + startingSpot + randomTree[index][index2] + 4.0), camera, wagonPos);
            tree.drawBillboard(glm::vec3(index2 + 0.5, 0.0, -index + startingSpot + randomTree[index][index2] + 6.0), camera, wagonPos);
         }
         else if (trailMap[index][index2] == LTTRAIL || trailMap[index][index2] == RTTRAIL
            || trailMap[index][index2] == RFARTRAIL || trailMap[index][index2] == RNEARTRAIL)
         {
            tree.draw(glm::vec3(index2, 0.0, -index + startingSpot - randomTree[index][index2]), camera, wagonPos);
            tree.draw(glm::vec3(index2 + 0.5, 0.0, -index + startingSpot - randomTree[index][index2] - 2.0), camera, wagonPos);
            tree.drawBillboard(glm::vec3(index2, 0.0, -index + startingSpot - randomTree[index][index2] - 4.0), camera, wagonPos);
            tree.drawBillboard(glm::vec3(index2 + 0.5, 0.0, -index + startingSpot - randomTree[index][index2] - 6.0), camera, wagonPos);
         }
      }
   }
}

Spline* Terrain::getSpline()
{
    return path;
}
