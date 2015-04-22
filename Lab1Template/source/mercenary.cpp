#include "mercenary.h"

// name generation tools
string First[] = {"Ricky", "Zdislav", "Dick", "Bryn", "Omari", 
		"Sizzle"};
int firstCount = 6;

string Last[] = {"Bobby", "Andrzejewski", "Zdrojewski", "Copperhordes", "Hugehair"};
int lastCount = 5;

string Title[] = {"Driver", "Bear-Hugger", "God", "Lord-Commander", "Pestilent", 
		"Determined", "Steadfast"};
int titleCount = 7;

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
	cost(30)
{
	cout << firstName + " " + lastName + ", the " + title + "\n";
}

void Mercenary::draw(GLint h_uModelMatrix)
{
	mesh.draw(h_uModelMatrix);
}
