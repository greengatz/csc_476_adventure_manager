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
                       	   "assets/tavern/door/door.obj",
                           "assets/tavern/bookshelf.obj",
                           "assets/tavern/stool.obj",
                           "assets/tavern/tableChair/tableChair.obj",
                           "assets/tavern/table/table.obj",
                       	   "assets/tavern/mug.obj",
                       	   "assets/tavern/torch.obj",
                           //"assets/tavern/stick.obj",
                           //"assets/tavern/pole.obj",
                           "assets/tavern/firepitlowpoly.obj",
                           "assets/tavern/turkey.obj",
               			   //"assets/tavern/roof.obj",
               			   //"assets/tavern/plank.obj",
               			   "assets/tavern/barrel/barrels.obj",
               			   "assets/tavern/mirror/mirrorFrame.obj",
               			   "assets/tavern/mirror/mirror.obj",
               			   "assets/tavern/stairs/stairs.obj",
               			   "assets/tavern/barstand/barstand.obj",
               			   "assets/tavern/barstand/jesusHolder.obj",
               			   "assets/tavern/roof/roof.obj"
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

const int NUM_PPL = 4;
const string pplFiles[] = {"assets/characters/samuraistatic.obj",
						   "assets/characters/spearmanstatic.obj",
                      	   "assets/tavern/landlord.obj",
                   		   "assets/characters/lumberjackstatic.obj",
                   		   "assets/events/monk.obj",
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
	cout << "Beginning mesh initialization:\n";

	//loads meshes only used in the tavern
	loadOnlyTavern();
	cout << "        Successully loaded tavern meshes\n";

	//loads meshes only used in the trail 
	loadOnlyTrail();
	cout << "        Successully loaded terrain meshes\n";
	
	//loads everything else	
	loadOther();
	cout << "        Successully loaded other meshes\n";

	//loads character meshes
	loadPpl(); 
	cout << "        Successully loaded people meshes\n";
}

void ProjectMeshes::loadOnlyTavern()
{
	printf("        Tavern meshes: ");
	float progress = (float)NUM_TAV / 2.0;
	for (int iter  = 0; iter < NUM_TAV; iter++) {
		if (iter > progress) {
			printf("%.2f%% -- ", progress / NUM_TAV * 100);
			progress += progress / 2.0;
		}
		addMesh(tavFiles[iter], &tavMeshes, false);
	}
	printf("DONE\n");
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
	printf("        People meshes: ");
	float progress = (float)NUM_PPL / 4.0;
	for (int iter  = 0; iter < NUM_PPL; iter++) {
		if (iter == SPEARMAN || iter == MONK)
			addMesh(pplFiles[iter], &pplMeshes, true);
		else 
			addMesh(pplFiles[iter], &pplMeshes, false);
		if (iter > progress) {
			printf("%.2f%% -- ", progress / NUM_PPL * 100);
			progress++;
		}
	}
	printf("DONE\n");
}