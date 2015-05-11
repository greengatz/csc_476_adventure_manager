#include "manager.h"

Manager::Manager(string newName)
{
	inTavern = true;
	name = newName;
	gold = 100.00;
	food = 0;
	beer = 0;

//	reportStats();
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
	    mercs[i].printDetails();
    }
}

void Manager::buyFood()
{
	//add food
	if (gold >= 5)
	{
		food++;
		gold -= 5;
		cout << "Bought 1 food for 5 gold" << endl;
	}
	else
	{
		cout << "Insufficient Funds" << endl;
	}

	reportStats();
}

void Manager::buyBeer()
{
	//add beer
	if (gold >= 2)
	{
		beer++;
		gold -= 2;
		cout << "Bought 1 beer for 2 gold\n";
	}
	else
	{
		cout << "Insufficient Funds" << endl;
	}
}

void Manager::buyMercenary(int mercenaryID, Tavern* tav)
{
	if(tav->tavernCharacters.size() >= mercenaryID && tav->tavernCharacters[mercenaryID].cost <= gold)
	{
		cout << "Buying " + tav->tavernCharacters[mercenaryID].firstName + "\n";
		mercs.push_back(tav->tavernCharacters[mercenaryID]);
		gold -= tav->tavernCharacters[mercenaryID].cost;
		tav->tavernCharacters.erase(tav->tavernCharacters.begin() + mercenaryID);
	}

	reportStats();
}

int Manager::getGold()
{
	return gold;
}

int Manager::getFood()
{
	return food; 
}

int Manager::getBeer()
{
	return beer;
}

int Manager::getMercs()
{
	return mercs.size();
}
