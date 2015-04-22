#include "mercenary.h"

#define HEALTH_VARIANCE 10
#define DAMAGE_VARIANCE 3
#define HUNGER_VARIANCE 5
#define BEER_VARIANCE 5


string JobNames[] = {"berzerker", "barbarian", "ranger", "cleric", "papiromancer",
        "magician"};


// name generation tools
string First[] = {"Zdislav", "Richard", "Bryn", "Omari", 
		"Sizzle", "Victoria", "Victor"};
int firstCount = 5;

string Last[] = {"Bobby", "Andrzejewski", "Zdrojewski", "Copperhordes", "Hugehair"};
int lastCount = 5;

string Title[] = {"Driver", "Bear-Hugger", "God", "Lord-Commander", "Pestilent", 
		"Determined", "Steadfast"};
int titleCount = 7;


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


Mercenary::Mercenary(Obj3d m) :
	mesh(m),
	firstName(randFirstName()),
	lastName(randLastName()),
	title(randTitle()),
    
    job(rand() % classes::size),
    
    maxHealth(BaseHealth[job] + rand() % HEALTH_VARIANCE),
    damage(BaseDamage[job] + rand() % DAMAGE_VARIANCE),
    hungerRate(BaseHungerRate[job] + rand() % HUNGER_VARIANCE),
    beerRate(BaseBeerRate[job] + rand() % BEER_VARIANCE),

	cost(30)
{
    printDetails();
}

void Mercenary::draw(GLint h_uModelMatrix)
{
	mesh.draw(h_uModelMatrix);
}

void Mercenary::printDetails()
{
	cout << firstName + " " + lastName + ", the " + title + "\n";
	cout << "   class: " + JobNames[job] + "\n";
	cout << "   health: " + to_string(static_cast<long long int>(maxHealth)) + "\n";
	cout << "   damage: " +  to_string(static_cast<long long int>(damage)) + "\n";
	cout << "   hunger rate: " +  to_string(static_cast<long long int>(hungerRate)) + "\n";
	cout << "   beer rate: " + to_string(static_cast<long long int>(beerRate)) + "\n";
	cout << "   cost: " +  to_string(static_cast<long long int>(cost)) + "\n";
}
