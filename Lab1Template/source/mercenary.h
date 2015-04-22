#ifndef MERCENARY_H
#define MERCENARY_H

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3d.h"

using namespace std;
using namespace glm;

// class definitions
enum classes {
    berzerker, // dps melee
    barbarian, // tank melee
    ranger, // generic ranged
    cleric, // healer
    papiromancer, // melee wizard
    magician, // wizard, hairy
    size
};

class Mercenary
{
	public:
		Obj3d mesh;
		// TODO make this a vector of meshes to hierarchically model
		// this can come from class

		string firstName;
		string lastName;
		string title;
		// character class
        int job;
		// Class, enum?
		int cost;

		// these are temporary, edit them as you need
		int maxHealth;
		int curHealth;
		int damage; // power if a healer

		int maxHunger;
		int currentHunger;
		int hungerRate;

		int maxHappiness;
		int currentHappiness;
		int beerRate;
	

		Mercenary(Obj3d mesh);
		void draw(GLint h_uModelMatrix);
		void update();
        void printDetails();
};

#endif
