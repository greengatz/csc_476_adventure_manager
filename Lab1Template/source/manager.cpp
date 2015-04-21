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
	int i;

	cout << name + "'s Stats:" << endl;
	cout << "Gold: " + to_string(static_cast<long double>(gold)) << endl;
	cout << "Beer: " + to_string(static_cast<long long int>(beer)) << endl;
	cout << "Food: " + to_string(static_cast<long long int>(food)) << endl;
	cout << "Mercenaries: " + to_string(static_cast<long long int>(mercs.size())) << endl;
	for (i = 0; i < mercs.size(); i++)
	{
		cout << "   " + mercs[i].firstName + " " + mercs[i].lastName +
			" the " + mercs[i].title + "\n";
	}
}
