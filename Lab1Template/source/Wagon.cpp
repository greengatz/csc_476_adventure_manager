/**
 * Wagon.cpp - Wagon along a trail.
 * @author Brandon Clark
 */

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Wagon.h"
#include "GLSL.h"
#include "splineCurve.h"
#include <math.h>

using namespace std;


string Ailment[] = {"small pox", "medieval depression", "mad cow disease", "bird flu", "a case of the frownies", 
                "soberitis"};
int ailmentCount = 6;

int MAX_MERC = 8;
int WAGON_TEX_ID = 111;
float neg = 1.0;
void (*fpFood)(void *, bool *) = NULL;
void (*fpBeer)(void *, bool *) = NULL;
void (*fpResume)(void *, bool *) = NULL;
void (*fpMercenary)(void *, bool *) = NULL;
void (*fpHeal)(void *, bool *) = NULL;
void (*fpHurt)(void *, bool *) = NULL;
void (*fpRob)(void *, bool *) = NULL;
void (*fpFight)(void *, bool *) = NULL;
void (*fpFlee)(void *, bool *) = NULL;
void (*fpReturnTavern)(void *, bool *) = NULL;
void (*fpRestartTrail)(void *, bool *) = NULL;

Wagon::Wagon() :
	position(0.6f, 0.05f, -0.5f),
	scale(0.03f, 0.03f, 0.03f),
  direction(0.0f, 0.0f, 0.0f),
  rotate(90.0f),
	posBufID(0),
	norBufID(0),
  indBufID(0),
	texBufID(0),
  startTime(0.0f),
  wagonStart(false),
  terrain(0),
  deltaTime(0.0f),
  velocity(0.75f),
  nextPoint(0.0f, 0.0f, 0.0f),
  orientation(1.0f, 0.0f, 0.0f)
{
}

Wagon::~Wagon()
{
}


void Wagon::init(TextureLoader* texLoader, Terrain* aTerrain, Menu* aMenu, bool* gP, Manager* mgr, ProjectMeshes* newData)
{
  float minX, minY, minZ;
  float maxX, maxY, maxZ;

   manager = mgr;
   gamePaused = gP;
   terrain = aTerrain;
   menu = aMenu;
   meshData = newData;

   //aPos relative to terrain generation
   resetWagon();

   // Load geometry
  // Some obj files contain material information.
  // We'll ignore them for this assignment.
  std::vector<tinyobj::material_t> objMaterials;
  std::string meshName = "assets/caravan/caravan.obj";
  string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str(),
    &minX, &maxX, &minY, &maxY, &minZ, &maxZ);
  if(!err.empty()) {
    cerr << err << endl;
  }

   // Send the position array to the GPU
   const vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufID);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

   //Set texture coordinates for the bark of the tree.
   const vector<float> &texBufRock = shapes[0].mesh.texcoords;
   glGenBuffers(1, &texBufID);
   glBindBuffer(GL_ARRAY_BUFFER, texBufID);
   glBufferData(GL_ARRAY_BUFFER, texBufRock.size()*sizeof(float), &texBufRock[0], GL_STATIC_DRAW);

   //Set the normals of the rock
   const vector<float> &norBuf = shapes[0].mesh.normals;
   glGenBuffers(1, &norBufID);
   glBindBuffer(GL_ARRAY_BUFFER, norBufID);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);


    //Load Texture
    texLoader->LoadTexture((char *)"assets/caravan/caravan_texture.bmp", WAGON_TEX_ID);

    //unbind the arrays
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

void Wagon::resetWagon()
{

    neg = 1.0;
    wagonStart = false;
    orientation = glm::vec3(1.0f, 0.0f, 0.0f);
    position = terrain->getStartPosition() + glm::vec3(0.6, 0.05, -0.5);
    // cout << "wagon is starting at " << position.x << ", " << position.z << "\n";
    nextPoint = terrain->nextCriticalPoint(position);
    direction = glm::normalize(nextPoint - position);
    terrain->printCriticalPoints();

    rotate = acos((glm::dot(direction, orientation)/(glm::length(orientation) * glm::length(direction)))) * (180.0/3.14);
}

void Wagon::startWagon()
{
  if (!wagonStart)
  {
    startTime = glfwGetTime();
    wagonStart = true;
  }
}

void buyFood(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  manager->buyFood(manager->medFoodCost);
}

void buyBeer(void* mgr, bool* gamePaused){
  // *gamePaused = false;

  Manager* manager = (Manager*)mgr;
  manager->buyBeer(manager->medBeerCost);
}

void buyMercenary(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->buyMercenary(manager->medGoldCost);
  *gamePaused = false;
}

void resumeGame(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  *gamePaused = false;
}

void healMercenary(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()
  // manager->mercs[index].setBeer(manager->mercs[index].getBeer() - cost);
  manager->setBeer(manager->getBeer() - manager->medBeerCost);
  manager->setFood(manager->getFood() - manager->medFoodCost);

  //SET FOCUS MERC IN MANAGER AND EDIT HIM
  *gamePaused = false;
}

void hurtMercenary(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()
  manager->lowerDamage(manager->focusedMerc);

  *gamePaused = false;
}

void robMerch(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()

  *gamePaused = false;
}

void fightAmbush(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()
  *gamePaused = false;
}

void restartTrail(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()
}

void returnTavern(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()
}

void fleeAmbush(void* mgr, bool* gamePaused){
  // *gamePaused = false;
  Manager* manager = (Manager*)mgr;
  // manager->healMercenary()

  int captured = manager->fleeingFromAmbush();
  cout << manager->getName(captured) + " has been captured!" << endl;
  *gamePaused = false;
}

void Wagon::updateWagon(float globalTime) {
  if (wagonStart && !terrain->atEnd(position)) {
    int event = terrain->checkEvents(position);
    if(manager->partyDead()){
      *gamePaused = true;
      vector<string> about;
      about.push_back("All your troops are dead.");
      about.push_back("Smooth move " + manager->getManagerName());
      //Create an option and add it to a vector
      fpRestartTrail = restartTrail;
      fpReturnTavern = returnTavern;
      option trailOpt = {"Restart trail", fpRestartTrail, false};
      option tavOpt = {"Return to previous tavern", fpReturnTavern, false};
      vector<option> options;
      options.push_back(trailOpt);
      options.push_back(tavOpt);

      //Set the data
      menu->setData("Deadsies", about, options);
    }
    if(event == MERCHANTEVENT){
      *gamePaused = true;
      int beerCost = (rand() % 4) + 2;
      int foodCost = (rand() % 4) + 2;
      manager->setMedFoodCost(foodCost);
      manager->setMedBeerCost(beerCost);
      //Create about vector and add an element
      vector<string> about;

      string aboutString = "Meat is  ";
      aboutString += to_string(manager->medFoodCost);
      aboutString += " gold and Beer is ";
      aboutString += to_string(manager->medBeerCost);
      aboutString += " gold!";
      about.push_back(aboutString);
      about.push_back("Merchant looks like a wimp though");
      //Create an option and add it to a vector
      fpFood = buyFood;
      fpBeer = buyBeer;
      fpResume = resumeGame;
      fpRob = robMerch;
      option foodOpt = {"Buy Meat", fpFood, false};
      option beerOpt = {"Buy Beer", fpBeer, false};
      option robOpt = {"Rob Merchant", fpRob, true};
      option resumeOpt = {"Continue On", fpResume, true};
      vector<option> options;
      options.push_back(foodOpt);
      options.push_back(beerOpt);
      options.push_back(robOpt);
      options.push_back(resumeOpt);

      //Set the data
      menu->setData("Merchant", about, options);

    }
    if(event == SICKNESS){
       *gamePaused = true;
      //Create about vector and add an element
      vector<string> about;
      srand(time(NULL));
      int beerCost = (rand() % 4) + 2;
      int foodCost = (rand() % 4) + 2;
      int index = manager->getRandomAliveMercIndex();
      manager->setFocus(index);
      manager->setMedFoodCost(beerCost);
      manager->setMedBeerCost(foodCost);
      
      string sickness = Ailment[rand() % ailmentCount];
      string name = manager->getName(index);
      cout << name + " just got " + sickness << endl;

      about.push_back(name + " came down with " + sickness + ",");
      vector<option> options;
      if(manager->getBeer() >= manager->medBeerCost &&
        manager->getFood() >= manager->medFoodCost){
        string aboutString = "Use ";
        aboutString += to_string(manager->medFoodCost);
        aboutString += " meat and ";
        aboutString += to_string(manager->medBeerCost);
        aboutString += " beer?";
        about.push_back(aboutString);
        about.push_back("Otherwise their damage will drop");
        fpHeal = healMercenary;
        fpHurt = hurtMercenary;
        option healOpt = {"Heal " + name, fpHeal, true};
        option hurtOpt = { name + " will be fine!", fpHurt, true};
        options.push_back(healOpt);
        options.push_back(hurtOpt);
      }else{
        about.push_back("But it looks like you don't have enough");
        about.push_back("beer and meat to treat them like they deserve!");
        fpResume = resumeGame;
        option resumeOpt = {"Continue On", fpResume, true};
        options.push_back(resumeOpt);
      }
      //Set the data
      menu->setData("Sickness", about, options);
    }
    if(event == WANDERER){
      vector<string> about;
      srand(time(NULL));
      int cost = (rand() % 20) + 25;
      int index = manager->getRandomAliveMercIndex();
      manager->setFocus(index);
      manager->setMedGoldCost(cost);
      *gamePaused = true;
      //Create about vector and add an element
      vector<option> options;
      // Obj3d temp(&((*meshData).terrMeshes[1]), scale, trans, rot);
      // Mercenary *newMerc = new Mercenary(meshData->);
      string name = manager->getName(index);
      about.push_back(name + " wants to join your party,");

      if(manager->getMercs() >= MAX_MERC){
        about.push_back("but your crew at max size!");
      }else if(manager->getGold() < cost){
        about.push_back("but you're a little short on gold!");
      }else{

        string aboutString = "buy this mercenary for ";
        aboutString += to_string(cost);
        aboutString += " gold?";
        about.push_back(aboutString);
        fpMercenary = buyMercenary;
        option mercOpt = {"Buy mercenary", fpMercenary, false};
        options.push_back(mercOpt);
      }
      fpResume = resumeGame;
      option resumeOpt = {"Continue On", fpResume, true};
      options.push_back(resumeOpt);

      //Set the data
      menu->setData("Wanderer", about, options);
    }
    if(event == AMBUSH){
      *gamePaused = true;
      //Create about vector and add an element
      vector<string> about;
      about.push_back("Bandits are ambushing your party");
      //Create an option and add it to a vector
      fpFight = fightAmbush;
      fpFlee = fleeAmbush;
      option fightOpt = {"Fight", fpFight, true};
      option fleeOpt = {"Flee", fpFlee, true};
      vector<option> options;
      options.push_back(fightOpt);
      options.push_back(fleeOpt);

      //Set the data
      menu->setData("Ambush", about, options);
    }
    
    deltaTime = glfwGetTime() - startTime;

    if (position.x >= nextPoint.x)
    {
      nextPoint = terrain->nextCriticalPoint(position);
      direction = glm::normalize(nextPoint - position);
      neg = -neg;
      rotate = neg * cos((glm::dot(direction, orientation)/(glm::length(orientation) * glm::length(direction)))) * (180.0/3.14);
    }
    if(*gamePaused == true){
      startTime = glfwGetTime() - deltaTime ;
    }else{
      position += direction * deltaTime * velocity;
    }
    position.y = 0.05;
    position.z = terrain->getSpline()->getY(position.x);
    rotate = 90.0f + -1.0 * atan(terrain->getSpline()->getDY(position.x)) * (180.0 / 3.14);

    startTime += deltaTime;
    }  
  
}

bool Wagon::hasStarted()
{
  return wagonStart;
}

void Wagon::setPosition(float x, float y, float z)
{
   position.x = x;
   position.y = y;
   position.z = z;
}

glm::vec3 Wagon::getPosition()
{
   return glm::vec3(position.x - 100, position.y, position.z);
}

void Wagon::setScale(glm::vec3 aScale)
{
   scale = aScale;
}

void Wagon::setTimeStamp(float newTime)
{
  startTime = newTime;
}

void Wagon::setRotation(float aRotation)
{
   rotate = aRotation;
}


void Wagon::draw(GLint h_pos, GLint h_nor, GLint h_aTexCoord, GLint h_ModelMatrix, RenderingHelper *modelTrans)
{
   //Position Wagon along the trail
   modelTrans->pushMatrix();
      modelTrans->translate(position);
      modelTrans->rotate(rotate, glm::vec3(0, 1, 0));
      modelTrans->scale(scale.x, scale.y, scale.z);
      glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(modelTrans->modelViewMatrix));
   modelTrans->popMatrix();

  //set up the texture unit
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, WAGON_TEX_ID);

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

   // Bind index array for drawing
   int nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_pos);
   GLSL::disableVertexAttribArray(h_nor);
   GLSL::disableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisable(GL_TEXTURE_2D);
}
