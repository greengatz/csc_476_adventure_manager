#pragma once
#ifndef _PROJECTMESHES_H_
#define _PROJECTMESHES_H_

#include "GLSL.h"
//#include "bounding.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3dcontainer.h"
#include "obj3d.h"
#include "mercenary.h"
#include "TextureLoader.h"
#include "Materials.h"
#include "FrustumCull.h"

using namespace std;

//tavern meshes
#define CUBE 0
#define DOOR 1
#define BOOKSHELF 2
#define STOOL 3
#define CHAIR 4
#define RECT_TABLE 5
#define MUG 6
#define TORCH 7
#define STICK 8
#define POLE 9
#define FIREPLACE 10
#define TABLEWARE 11
#define TURKEY 12
#define ROCK 13
#define BALUSTRADE 14
#define ROOF 15
#define PLANK 16
#define LANDLORD 17
#define LUMBERJACK 18

//terrain meshes
#define START_CITY 0
#define END_CITY 1
#define ROPE 2
#define STALL 3
#define MERCHANT 4

//other meshes
#define CRATE 0
#define BARREL 1
#define BOTTLE 2

//ppl meshes
#define SAMURAI 0
#define SPEARMAN 1

class ProjectMeshes
{
	public:
		vector<Obj3dContainer> tavMeshes;
		vector<Obj3dContainer> terrMeshes;
		vector<Obj3dContainer> otherMeshes;
		vector<Obj3dContainer> pplMeshes;
		ProjectMeshes();
		// void init();
		void loadMeshes();

	private:
		void addMesh(const string filename, vector<Obj3dContainer> *target, bool noNorms);
		void loadOnlyTavern(); //loads meshes only used in the tavern
		void loadOnlyTrail(); //loads meshes only used in the trail
		void loadOther(); //loads everything else
		void loadPpl(); //loads the people meshes this may be deleted later depending on how we load them
};
#endif
