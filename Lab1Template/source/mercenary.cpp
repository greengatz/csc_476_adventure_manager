#include "mercenary.h"

#define HEALTH_VARIANCE 10
#define DAMAGE_VARIANCE 3
#define HUNGER_VARIANCE 5
#define BEER_VARIANCE 5


string JobNames[] = {"berzerker", "barbarian", "ranger", "cleric", "papiromancer",
        "magician"};


// name generation tools
string First[] = {"Zdislav", "Richard", "Bryn", "Omari", "Sizzle", 
                "Victoria", "Victor", "Florence", "Napolean"};
int firstCount = 9;

string Last[] = {"Bobby", "Andrzejewski", "Zdrojewski", "Copperhordes", "Hugehair"
                "Bourchier", "Cardonell", "de La Chapagne", "Dreux"};
int lastCount = 9;

string Title[] = {"Driver", "Bear-Hugger", "God", "Lord-Commander", "Pestilent", 
		"Determined", "Steadfast", "Untrustworthy", "Teletubby"};
int titleCount = 9;


// base stats
int BaseHealth[] = {35, 50, 25, 28, 20, 20};
int BaseDamage[] = {10, 3, 6, 10, 15, 10};
int BaseCost[] = {25, 25, 25, 25, 25, 25};

int BaseHungerRate[] = {5, 5, 5, 5, 5, 5};
int BaseBeerRate[] = {5, 5, 5, 5, 5, 5};


string randFirstName() {
	return First[rand() % firstCount];
}

string randLastName() {
	return Last[rand() % lastCount];
}

string randTitle() {
	return Title[rand() % titleCount];
}


Mercenary::Mercenary(vector<Obj3d> m) :
	meshes(m),
	firstName(randFirstName()),
	lastName(randLastName()),
	title(randTitle()),
    
    job(rand() % classes::size),
    
    maxHealth(BaseHealth[job] + rand() % HEALTH_VARIANCE),
    damage(BaseDamage[job] + rand() % DAMAGE_VARIANCE),
    hungerRate(BaseHungerRate[job] + rand() % HUNGER_VARIANCE),
    beerRate(BaseBeerRate[job] + rand() % BEER_VARIANCE),

	cost(BaseCost[job]),

    isWaving(false)
{
    cost += maxHealth - BaseHealth[job];
    cost += damage - BaseDamage[job];
    cost -= hungerRate - BaseHungerRate[job];
    cost -= beerRate - BaseBeerRate[job];

    curHealth = maxHealth;
}

void Mercenary::draw(GLint h_uModelMatrix, int meshIndex)
{
    // animate here
    clock_t delta = clock();

    if((delta - animationStart) > 1000000 && isWaving) {
        isWaving = false;
    }

    if(isWaving) {
        float armAngle = 0.0f;
        if((delta - animationStart) < 500000) {
            armAngle = ((float) (delta - animationStart)) / 5000.0f;
        } else {
            armAngle = (100.0f) - (((float) (delta - animationStart - 500000)) / 5000.0f);
        }
        meshes[1].rot = glm::rotate(glm::mat4(1.0f), -armAngle, glm::vec3(0, 0, 1.0f));
    }

    meshes[meshIndex].draw(h_uModelMatrix);
}

void Mercenary::printDetails()
{
	cout << firstName + " " + lastName + ", the " + title + "\n";
	cout << "   class: " + JobNames[job] + "\n";
	cout << "   health: " + to_string(static_cast<long long int>(curHealth)) + "/" + to_string(static_cast<long long int>(maxHealth)) + "\n";
	cout << "   damage: " +  to_string(static_cast<long long int>(damage)) + "\n";
	cout << "   hunger rate: " +  to_string(static_cast<long long int>(hungerRate)) + "\n";
	cout << "   beer rate: " + to_string(static_cast<long long int>(beerRate)) + "\n";
	cout << "   cost: " +  to_string(static_cast<long long int>(cost)) + "\n";
}

void Mercenary::wave() {
    if(!isWaving) {
        meshes[1].rot = glm::rotate(glm::mat4(1.0f), -50.0f, glm::vec3(0, 0, 1.0f));
        animationStart = clock();
        isWaving = true;
    }
}
