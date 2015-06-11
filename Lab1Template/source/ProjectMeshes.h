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
#define FIREPLACE 8
#define TURKEY 9
#define BARRELS 10
#define MIRRORFRAME 11
#define MIRROR 12
#define STAIRS 13
#define BARSTAND 14
#define JESUSHOLDER 15
#define TAVERNROOF 16

//terrain meshes
#define START_CITY 0
#define END_CITY 1
#define ROPE 2
#define STALL 3
#define VENDOR 4

//other meshes
#define CRATE 0
#define BARREL 1
#define BOTTLE 2

//ppl meshes
#define SAMURAI 0
#define SPEARMAN 1
#define LANDLORD 2
#define LUMBERJACK 3
#define MONK 4

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
