#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "GLSL.h"
#include "Camera.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3dcontainer.h"
#include "obj3d.h"
#include "mercenary.h"
#include "tavern.h"

using namespace std;
using namespace glm;

class Manager
{
	public:
		
		bool inTavern;
		Manager(string name);
		void buyMercenary(int key, Tavern* tavern);
		void reportStats();
		void buyFood();
		void buyBeer();
		int getGold();
		int getFood();
		int getBeer();
		int getMercs();


	private:
		vec3 position;
		string name;
		int gold;
		int food;
		int beer;
		vector<Mercenary> mercs;
};

#endif
