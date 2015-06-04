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
		void tickHungerHealth();
		void beerMerc(int index);
		void feedMerc(int index);
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

		tdogl::pngTexture* oopsRanMenu;
		tdogl::pngTexture* noGoldWandererMenu;
		tdogl::pngTexture* merchantRobbedMenu;
		tdogl::pngTexture* fightWonMenu;
		tdogl::pngTexture* diedMenu;
		tdogl::pngTexture* cowardMenu;
		
		

};

#endif
