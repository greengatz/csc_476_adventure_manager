#include "manager.h"

Manager::Manager(string newName)
{
	name = newName;
	gold = 100.00;
	food = 0;
	beer = 0;
	mercs = 0;
}

void Manager::reportStats()
{
	cout << name + "'s' Stats:" << endl;
	cout << "Gold: " + to_string(gold) << endl;
	cout << "Beer: " + to_string(beer) << endl;
	cout << "Mercenarys: " + to_string(mercs) << endl;
}