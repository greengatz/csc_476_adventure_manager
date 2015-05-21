#include "manager.h"

Manager::Manager(string newName)
{
	inTavern = true;
	name = newName;
	gold = 100.00;
	food = 0;
	beer = 0;
	focusedMerc = 0;
	medFoodCost = 5;
	medBeerCost = 2;
//	reportStats();
}


string Manager::getManagerName(){
	return name;
}

void Manager::setFocus(int index){
	focusedMerc = index;
}

Mercenary Manager::getFocus(){
	return mercs[focusedMerc];
}

bool Manager::partyDead(){
	for (int i = 0; i < mercs.size(); i++)
	{
	    if(mercs[i].dead == false)
	    	return false;
    }
    return true;
}

int Manager::getRandomAliveMercIndex(){
	srand(time(NULL));
	bool found = false;
	int index = 0;
	while (!found)
	{
	    index = rand() % mercs.size();
	    cout << "Mercenary index: " + to_string(index) << endl;
	    if(mercs[index].dead == false){
	    	found = true;
	    }
    }
    return index;
}

int Manager::fleeingFromAmbush(){
	srand(time(NULL));
	bool found = false;
	int index = 0;
	while (!found)
	{
	    index = rand() % mercs.size();
	    cout << "Mercenary index: " + to_string(index) << endl;
	    if(mercs[index].dead == false){
	    	mercs[index].dead = true;
	    	found = true;
	    }
    }
    return index;
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

string Manager::getName(int index)
{
	return mercs[index].firstName;
}

void Manager::buyFood(int cost)
{
	//add food
	if (gold >= cost)
	{
		food++;
		gold -= cost;
		cout << "Bought 1 food for " + to_string(static_cast<long double>(cost)) + " gold" << endl;
	}
	else
	{
		cout << "Insufficient Funds" << endl;
	}

	reportStats();
}

void Manager::buyBeer(int cost)
{
	//add beer
	if (gold >= cost)
	{
		beer++;
		gold -= cost;
		cout << "Bought 1 beer for " + to_string(static_cast<long double>(cost)) + " gold\n";
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

// void Manager::buyMercenaryTrail(int mercenaryID. int cost)
// {
// 	if(gold >= cost){
// 		Mercenary merc = new Mercenary();
// 		cout << "Buying " + tav->tavernCharacters[mercenaryID].firstName + "\n";
// 		mercs.push_back(tav->tavernCharacters[mercenaryID]);
// 		gold -= tav->tavernCharacters[mercenaryID].cost;
// 		tav->tavernCharacters.erase(tav->tavernCharacters.begin() + mercenaryID);
// 	}

// 	reportStats();
// }

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

void Manager::setGold(int newGold)
{
	if(newGold > 0){
		gold = newGold;
	}
}

void Manager::setFood(int newFood)
{
	if(newFood > 0){
		food = newFood; 
	}
}

void Manager::setBeer(int newBeer)
{
	if(newBeer > 0){
		beer = newBeer;
	}
}

void Manager::lowerDamage(int index)
{
	mercs[index].currDamage -= floor((float)(mercs[index].currDamage / 2.0));
}

int Manager::reportTotalDamage() {
   int damage = 0, i;
   for (i = 0; i < mercs.size(); i++)
	{
	    damage += mercs[i].currDamage;
    }
    return damage;
}

void Manager::setMedFoodCost(int cost){
	medFoodCost = cost;
}

void Manager::setMedBeerCost(int cost){
	medBeerCost = cost;
}

void Manager::setMedGoldCost(int cost){
	medGoldCost = cost;
}
