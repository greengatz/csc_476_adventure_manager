#include "mercenary.h"
#include <cmath>

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
                "Bourchier", "Cardonell", "del Chapagne", "Dreux"};
int lastCount = 9;

string Title[] = {"Driver", "Bear-Hugger", "God", "Lord-Commander", "Pestilent", 
		"Determined", "Steadfast", "Untrustworthy", "Teletubby"};
int titleCount = 9;


// base stats
int BaseHealth[] = {35, 50, 25, 28, 20, 20};
int BaseDamage[] = {10, 3, 6, 10, 15, 10};
int BaseCost[] = {25, 25, 25, 25, 25, 25};

int BaseHungerRate[] = {15, 15, 15, 15, 15, 15};
int BaseBeerRate[] = {15, 15, 15, 15, 15, 15};

// Dae location
string fileLoc[] = {"assets/characters/noAnim.dae", 
        "assets/characters/noAnim.dae", "assets/characters/noAnim.dae", 
        "assets/characters/noAnim.dae", "assets/characters/noAnim.dae", 
        "assets/characters/noAnim.dae", "assets/characters/noAnim.dae"};

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
    job(rand() % 7),
    dead(false),
    maxHealth(BaseHealth[job] + rand() % HEALTH_VARIANCE),
    maxDamage(BaseDamage[job] + rand() % DAMAGE_VARIANCE),
    maxHunger(BaseHungerRate[job] + rand() % HUNGER_VARIANCE),
    maxHappiness(BaseBeerRate[job] + rand() % BEER_VARIANCE),

    cost(BaseCost[job]),

    isWaving(false)
{
    cost += maxHealth - BaseHealth[job];
    cost += maxDamage - BaseDamage[job];
    cost -= maxHunger - BaseHungerRate[job];
    cost -= maxHunger - BaseBeerRate[job];
    currDamage = maxDamage;
    currHealth = maxHealth;
    currHunger = maxHunger;
    currHappiness = maxHappiness;
}

Mercenary::Mercenary() :
    firstName(randFirstName()),
    lastName(randLastName()),
    title(randTitle()),
    
    job(rand() % 7),
    dead(false),
    maxHealth(BaseHealth[job] + rand() % HEALTH_VARIANCE),
    maxDamage(BaseDamage[job] + rand() % DAMAGE_VARIANCE),
    maxHunger(BaseHungerRate[job] + rand() % HUNGER_VARIANCE),
    maxHappiness(BaseBeerRate[job] + rand() % BEER_VARIANCE),

    cost(BaseCost[job]),

    isWaving(false)
{
    cost += maxHealth - BaseHealth[job];
    cost += maxDamage - BaseDamage[job];
    cost -= maxHunger - BaseHungerRate[job];
    cost -= maxHunger - BaseBeerRate[job];
    currDamage = maxDamage;
    currHealth = maxHealth;
    dae = NULL;
    currHunger = maxHunger;
    currHappiness = maxHappiness;
}

void Mercenary::draw(GLint h_uModelMatrix, int meshIndex)
{
    // animate here
    clock_t delta = clock();
    if(currHealth == 0){
        dead = true;
    }
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
	cout << "   Class: " + JobNames[job] + "\n";
	cout << "   Health: " + to_string(static_cast<long long int>(currHealth)) + "/" + to_string(static_cast<long long int>(maxHealth)) + "\n";
	cout << "   Damage: " +  to_string(static_cast<long long int>(currDamage)) + "\n";
	cout << "   Hunger: " +  to_string(static_cast<long long int>(currHunger)) + "\n";
    cout << "   Happiness: " + to_string(static_cast<long long int>(currHappiness)) + "\n";
    cout << "   CalcDamage: " + to_string(static_cast<long long int>(Mercenary::calcDamage())) + "\n";
	cout << "   Cost: " +  to_string(static_cast<long long int>(cost)) + "\n";
}

void Mercenary::wave() {
    if(!isWaving) {
/*        meshes[1].rot = glm::rotate(glm::mat4(1.0f), -50.0f, glm::vec3(0, 0, 1.0f));
        animationStart = clock();
        isWaving = true;*/
    }
}

void Mercenary::initDae() {
    job = job < 0 ? -job : job;
    dae = new CharDae(fileLoc[job % 7]);
}

int Mercenary::calcDamage(){
    return floor((currDamage + currHunger + currHappiness) / 3.0);
}
