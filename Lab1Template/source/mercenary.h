#ifndef MERCENARY_H
#define MERCENARY_H

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3d.h"
#include "charDae.h"
#include <time.h>

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
		vector<Obj3d> meshes;
		// TODO load these from our classes

        CharDae* dae;

		string firstName;
		string lastName;
		string title;
		
        // character class
        int job;
		int cost;

		// these are temporary, edit them as you need
		int maxHealth;
		int currHealth;
		int maxDamage; // power if a healer
		int currDamage;

		int maxHunger;
		int currentHunger;
		int hungerRate;

		int maxHappiness;
		int currentHappiness;
		int beerRate;
        bool dead;

        // method prototypes
		Mercenary(vector<Obj3d> mesh);
		Mercenary();
		void draw(GLint h_uModelMatrix, int meshIndex);
		void update();
        void printDetails();
        void wave();
        void initDae();

    private:
        clock_t animationStart;
        bool isWaving;
};

#endif
