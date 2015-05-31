#include "ProjectMeshes.h"

/*ADDING A NEW MESH:
 * 1) Add to array where it is being used, if you dont know then just add it to other
 * 2) Increment the number for the respective array (ie if adding to other increment NUM_OTHER)
 * 3) Add a #define to ProjectMeshes.h in the appropriate section 
 * 4) Make sure when you try to access call the correct array and then use the #define
 *    (ie to get the box.obj call otherMeshes[BOX])
 *
 * NOTE: If the obj does not have normals already in it, tell Daniel to add it or add normals to the file
 */

const int NUM_TAV = 17;
const string tavFiles[] = {"assets/tavern/cube.obj",
                       	   "assets/tavern/door.obj",
                           "assets/tavern/bookshelf.obj",
                           "assets/tavern/stool.obj",
                           "assets/tavern/chair.obj",
                           "assets/tavern/table.obj",
                       	   "assets/tavern/mug.obj",
                       	   "assets/tavern/torch.obj",
                           "assets/tavern/stick.obj",
                           "assets/tavern/pole.obj",
                           "assets/tavern/firepitlowpoly.obj",
                           // "assets/tavern/tableware.obj", //incorporate later?
                           "assets/tavern/turkey.obj",
               			   "assets/tavern/rock.obj",
               			   "assets/tavern/roof.obj",
               			   "assets/tavern/plank.obj",
                      	   "assets/tavern/landlord.obj",
                   		   "assets/tavern/lumberjack.obj",
               			   // "assets/tavern/balustrade.obj",
};

const int NUM_TERR = 5;
const string terrFiles[] = {"assets/events/startCity.obj",
							"assets/events/endCity.obj",
							"assets/events/rope.obj",
							"assets/events/stall.obj",
							"assets/events/merchant.obj",
};

const int NUM_OTHER = 3;
const string otherFiles[] = {"assets/tavern/box.obj",
	                         "assets/tavern/barrel.obj",
                             "assets/tavern/bottle.obj",
};

const int NUM_PPL = 2;
const string pplFiles[] = {"assets/events/samurai.obj",
						   "assets/events/spearman.obj",
};

ProjectMeshes::ProjectMeshes()
{

}

void ProjectMeshes::addMesh(const string filename, vector<Obj3dContainer> *target, bool noNorms)
{
	Obj3dContainer temp;
	temp.loadIntoTinyOBJ(filename);
	temp.initBuffers(noNorms);
	(*target).push_back(temp);
}

void ProjectMeshes::loadMeshes()
{
	//loads meshes only used in the tavern
	loadOnlyTavern();
	cout << "Successully loaded tavern meshes\n";

	//loads meshes only used in the trail 
	loadOnlyTrail();
	cout << "Successully loaded terrain meshes\n";
	
	//loads everything else	
	loadOther();
	cout << "Successully loaded other meshes\n";

	//loads character meshes
	loadPpl(); 
	cout << "Successully loaded people meshes\n";
}

void ProjectMeshes::loadOnlyTavern()
{
	float progress = (float)NUM_TAV / 2.0;
	for (int iter  = 0; iter < NUM_TAV; iter++) {
		if (iter > progress) {
			printf("Tavern meshes at %.2f%%\n", progress / NUM_TAV * 100);
			progress += progress / 2.0;
		}
		addMesh(tavFiles[iter], &tavMeshes, false);
	}
}

void ProjectMeshes::loadOnlyTrail()
{
	for (int iter  = 0; iter < NUM_TERR; iter++) {
		addMesh(terrFiles[iter], &terrMeshes, false);
	}
}

void ProjectMeshes::loadOther()
{
	for (int iter  = 0; iter < NUM_OTHER; iter++) {
		if (iter == BARREL)
			addMesh(otherFiles[iter], &otherMeshes, true);
		else 
			addMesh(otherFiles[iter], &otherMeshes, false);
	}
}

void ProjectMeshes::loadPpl()
{
	for (int iter  = 0; iter < NUM_PPL; iter++) {
		if (iter == SPEARMAN)
			addMesh(pplFiles[iter], &pplMeshes, true);
		else 
			addMesh(pplFiles[iter], &pplMeshes, false);
	}
}