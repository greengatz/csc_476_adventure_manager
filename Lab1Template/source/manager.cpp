#include "manager.h"

Manager::Manager(string newName, Camera cam)
{
	camera = cam;
	name = newName;
	gold = 100.00;
	food = 0;
	beer = 0;
	//mercs = 0;
	// this should be a list of an abstract type

	reportStats();
}

void Manager::reportStats()
{
	cout << name + "'s Stats:" << endl;
	cout << "Gold: " + to_string(static_cast<long double>(gold)) << endl;
	cout << "Beer: " + to_string(static_cast<long long int>(beer)) << endl;
	cout << "Mercenarys: " + to_string(static_cast<long long int>(mercs.size())) << endl;
}
