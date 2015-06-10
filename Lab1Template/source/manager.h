#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "GLSL.h"
#include "Camera.h"
#include "math.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3dcontainer.h"
#include "ProjectMeshes.h"
#include "obj3d.h"
#include "mercenary.h"
#include "tavern.h"
#include "FadeSystem.h"
#include "SoundPlayer.h"
#include "pngTexture.h"
#include "Bitmap.h"
#include "stb_image.h"

using namespace std;
using namespace glm;

class Menu;

class Manager
{
	public:
		int focusedMerc;
		int medFoodCost;
		int medBeerCost;
		int medGoldCost;
		bool inTavern;
		bool blacklisted;
		bool fortune;
		Manager(string name);
		void init(Menu *m, bool *gp, FadeSystem *fS, SoundPlayer *aud);
		void buyMercenary(int key, Tavern* tavern);
		void buyMercenaryTrail(int cost);
		void reportStats();
		void buyFood(int cost);
		void buyBeer(int cost);
		string getName(int index);
		string getManagerName();
		int getGold();
		int getFood();
		int getBeer();
		int getMercs();
		void setGold(int newGold);
		void setFood(int newFood);
		void setBeer(int newBeer);
		void setFocus(int index);
		void lowerDamage(int index);
		int reportTotalDamage();
		void setMedFoodCost(int cost);
		void setMedBeerCost(int cost);
		void setMedGoldCost(int cost);
		int getRandomAliveMercIndex();
		void fleeingFromAmbush();
		void fightingFromAmbush(int numBandits, int banditDamage);
		void fightingFromMerchant(int numGaurds, int gaurdDamage);
		Mercenary getFocus();
		bool partyDead();
		void drawMenuManager();
		void restartFromTrail();
		void restartFromTavern();
		bool getInMenu();
		void completedTrail();
		void tickHungerHealth();
		void beerMerc();
		void feedMerc();
		SoundPlayer* getAudio();
		FadeSystem* getFade();
		vector<Mercenary> mercs;

	private:
		vec3 position;
		string name;
		int gold;
		int food;
		int beer;
		Menu *menu;
		bool *gamePaused;
		FadeSystem *fadeSystem;
		SoundPlayer *audio;

		tdogl::pngTexture* oopsRanMenu; //6
		tdogl::pngTexture* noGoldWandererMenu; //7
		tdogl::pngTexture* merchantRobbedMenu; //8
		tdogl::pngTexture* fightWonMenu; //9
		tdogl::pngTexture* diedMenu; //10
		tdogl::pngTexture* cowardMenu; //11
		
		

};

#endif
