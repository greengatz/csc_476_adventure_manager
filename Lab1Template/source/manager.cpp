#include "manager.h"
#include "menu.h"

void (*fpContinue)(void *, bool *) = NULL;
void (*fpReturnTavern)(void *, bool *) = NULL;
void (*fpRestartTrail)(void *, bool *) = NULL;


Manager::Manager(string newName)
{
	blacklisted = false;
	fortune = false;
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

void Manager::init(Menu *m, bool *gp, FadeSystem *fS, SoundPlayer *aud){
	menu = m;
	gamePaused = gp;
	fadeSystem = fS;
	audio = aud;

	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile("assets/oopsRanMenu.png");
	oopsRanMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);
	

	// bmp = tdogl::Bitmap::bitmapFromFile("assets/merchantRobbed.png");
	bmp = tdogl::Bitmap::bitmapFromFile("assets/robberyMenu.png");
	merchantRobbedMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

	

	bmp = tdogl::Bitmap::bitmapFromFile("assets/fightWonMenu.png");
	fightWonMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

	bmp = tdogl::Bitmap::bitmapFromFile("assets/diedMenu.png");
	diedMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

	bmp = tdogl::Bitmap::bitmapFromFile("assets/cowardMenu.png");
	cowardMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

	bmp = tdogl::Bitmap::bitmapFromFile("assets/skilledMerchantMenu.png");
	skilledMerchantMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);

	bmp = tdogl::Bitmap::bitmapFromFile("assets/trailConqueredMenu.png");
	trailConqueredMenu = new tdogl::pngTexture(bmp, GL_LINEAR, GL_REPEAT);


	

	
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
	bool found = false;
	int index, i;
	index = rand() % mercs.size();

	for(i = 0; i < mercs.size(); i++)
	{
	    if(mercs[i].dead == false){
	    	found = true;
	    }
    }

    if(found){
    	found = false;
    	while(!found){
    		if(mercs[index].dead == false){
    			found = true;
    		}else{
    			index = rand() % mercs.size();
    		}
    	}
    	return index;
    }

    return -1;
}

void continueGame(void* mgr, bool* gamePaused){
  *gamePaused = false;
}


void collectReward(void* mgr, bool* gamePaused){
	Manager* manager = (Manager*)mgr;
	manager->setGold(manager->getGold() + manager->medGoldCost);
	manager->setBeer(manager->getBeer() + manager->medBeerCost);
	manager->setFood(manager->getFood() + manager->medFoodCost);
  	*gamePaused = false;
}

void Manager::completedTrail(){
	int i = 0;
	for(i = 0; i < mercs.size(); i++){
		if(mercs[i].dead){
			mercs.erase(mercs.begin() + i);
		}else{
			mercs[i].dead = false;
			mercs[i].sick = false;
			mercs[i].currHealth = mercs[i].maxHealth;
			mercs[i].currHealth = mercs[i].maxHealth;
			mercs[i].currHappiness = mercs[i].maxHappiness;
			mercs[i].currHunger = mercs[i].maxHunger;
		}
	}
	// trailConqueredMenu HERE!!!!!!!!!!
	setMedFoodCost(5);
	setMedBeerCost(5);
	setMedGoldCost(50);
	vector<string> dataStuffs;
    vector<string> about;
    about.push_back("Congratulations on overcoming the trail!");  
    about.push_back("As a reward, take 50gold, 5 beer, and 5 meat!");  
    //Create an option and add it to a vector
    fpContinue = collectReward;
    // fpResume = resumeGame;
    option resumeOpt = {"Collect", fpContinue, true};
    vector<option> options;
    options.push_back(resumeOpt);
    //Set the data
    menu->setData("Trail Conquerer", about, options, &trailConqueredMenu, 13, dataStuffs);
}

void Manager::restartFromTrail(){
	int i = 0;
	for(i = 0; i < mercs.size(); i++){
		mercs[i].currHealth = mercs[i].maxHealth;
		mercs[i].currHappiness = mercs[i].maxHappiness;
		mercs[i].currHunger = mercs[i].maxHunger;
		mercs[i].dead = false;
		mercs[i].sick = false;
        mercs[i].dae->lastAnim = 0;
	}
	gold = 30;
	beer = 4;
	food = 4;
	
}


void Manager::restartFromTavern(){
	int i = 0;
	for(i = 0; i < mercs.size(); i++){
		if(mercs[i].dead){
			mercs.erase(mercs.begin() + i);
		}else{
            mercs[i].dae->lastAnim = 0;
			mercs[i].dead = false;
			mercs[i].sick = false;
			mercs[i].currHealth = mercs[i].maxHealth;
			mercs[i].currHappiness = mercs[i].maxHappiness;
			mercs[i].currHunger = mercs[i].maxHunger;
		}
	}
	gold = 100;
	beer = 0;
	food = 0;
}


void restartTrail(void* mgr, bool* gamePaused){
  // *gamePaused = false;
	Manager *manager = (Manager *)mgr;
	manager->inTavern = false;
	manager->getFade()->dontToggleView = true;
	manager->getFade()->startFade(1024, 768);
	manager->getAudio()->playBackgroundMusic(manager->inTavern);
	manager->restartFromTrail();
}

void returnTavern(void* mgr, bool* gamePaused){
  // *gamePaused = false;
	Manager *manager = (Manager *)mgr;
	manager->inTavern = true;
	manager->getAudio()->playBackgroundMusic(manager->inTavern);
	manager->getFade()->startFade(1024, 768);
	*gamePaused = false;
	manager->restartFromTavern();
}

void Manager::fleeingFromAmbush(){
	
	srand(time(NULL));
	bool found = false;
	int index = 0;
	int goldLoss = (rand() % 10 )+ 1, oldGold = gold;
	int foodLoss = (rand() % 2 ) + 1, oldMeat = food;
	int beerLoss = (rand() % 2 ) + 1, oldBeer = beer;
	int randDamage = (rand() % 20) + 10;
	vector<string> dataStuffs;
 	vector<string> about;
 	*gamePaused = true;
	about.push_back("While you were cowardly fleeing the bandits,");

	
	if(gold - goldLoss <= 0){
		goldLoss = gold;
		gold = 0;
	}else{
		gold = gold - goldLoss;
	}
	if(food - foodLoss <= 0){
		foodLoss = food;
		food = 0;
	}else{
		food = food - foodLoss;
	}
	if(beer - beerLoss <= 0){
		beerLoss = beer;
		beer = 0;
	}else{
		beer -= beerLoss;
	}
	dataStuffs.push_back(to_string(goldLoss));
	about.push_back("They took " + to_string((goldLoss)) + ",");

	if((oldBeer - beer < 1 && oldMeat - food < 1 && oldGold - gold < 5)){
		int i = Manager::getRandomAliveMercIndex();
		if(i >= 0){
			mercs[i].currHealth -= randDamage;
			if(mercs[i].currHealth <= 0){
				mercs[i].currHealth = 0;
				mercs[i].dead = true;
				string aboutString = "and sent " + mercs[i].firstName + "to swim with the fishes.";
				about.push_back(aboutString);
			}else{
				string aboutString = "and roughed up  " + mercs[i].firstName + " a bit!";
				about.push_back(aboutString);
			}
		}
	}else{

		
		string aboutString = "They took " + to_string((goldLoss)) + "gold,";
		about.push_back(aboutString);
		dataStuffs.clear();
		dataStuffs.push_back(to_string(goldLoss));
	
		aboutString = ", " + to_string(foodLoss) + "food, and" + to_string(beerLoss) + "beer!";
		about.push_back(aboutString);
		dataStuffs.push_back(to_string(foodLoss));
		dataStuffs.push_back(to_string(beerLoss));
	}

    fpContinue = continueGame;
    option resumeOpt = {"Continue On", fpContinue, true};
    vector<option> options;
    options.push_back(resumeOpt);
    //Set the data
    menu->setData("Coward", about, options, &cowardMenu, 11, dataStuffs);
    if(menu->inMenu){
    	printf("In mesnu is true!\n");
    }
}

void Manager::fightingFromAmbush(int numBandits, int banditDamage){
	
	srand(time(NULL));
	int index = 0;
	int goldGain = (rand() % 5 ) + 10;
	int foodGain = (rand() % 2 ) + 1;
	int beerGain = (rand() % 2 ) + 1;

	int incomingDamage = 0; 
	int defendingDamage = 0;

	vector<string> dataStuffs;
 	vector<string> about;
 	*gamePaused = true;
	
	for(index = 0; index < numBandits; index++){
		incomingDamage += ((rand() % 3) - 1) + banditDamage;
	}
	incomingDamage *= 2;
	for(index = 0; index < mercs.size(); index++){
		defendingDamage += mercs[index].calcDamage();
	}

	incomingDamage -= defendingDamage;

	Manager::reportStats();
	cout << "Incoming damage: " + to_string(incomingDamage) << endl;
	

	for(index = 0; index < incomingDamage; index++){
		int i = Manager::getRandomAliveMercIndex();
		if(i >= 0){
			if(mercs[i].currHealth == 0){
				mercs[i].dead = true;
			}else{
				mercs[i].currHealth--;
			}
		}
	}

	if(Manager::getRandomAliveMercIndex() < 0){
		string aboutString = "The bandits crushed your whole team";
			about.push_back(aboutString);
	    fpReturnTavern = returnTavern;
	    fpRestartTrail = restartTrail;
	    option resumeOpt = {"Restart from trail", fpRestartTrail, true};
	    option resume2Opt = {"Restart from tavern", fpReturnTavern, true};
	    vector<option> options;
	    options.push_back(resumeOpt);
	    options.push_back(resume2Opt);
	    //Set the data
	    menu->setData("Close but no pipe", about, options, &diedMenu, 10, about);
	}else{
		gold += goldGain;
		food += foodGain;
		beer += beerGain;
		string aboutString = "You conquered those bandit scumbags!";
		about.push_back(aboutString);
		dataStuffs.push_back(to_string(goldGain));
		dataStuffs.push_back(to_string(beerGain));
		dataStuffs.push_back(to_string(foodGain));
		aboutString = "You gained " + to_string(goldGain) + "gold, " + to_string(beerGain) + "beer, " + to_string(foodGain) + "meat!";
		about.push_back(aboutString);
	    fpContinue = continueGame;
	    option resumeOpt = {"Continue On", fpContinue, true};
	    vector<option> options;
	    options.push_back(resumeOpt);
	    //Set the data
	    menu->setData("Great slayin' ", about, options, &fightWonMenu, 9, dataStuffs);
	}
		
	Manager::reportStats();
		
}

void Manager::fightingFromMerchant(int numGaurds, int gaurdDamage){
	
	srand(time(NULL));
	int index = 0;
	blacklisted = true;
	int goldGain = (rand() % 40 ) + 20;
	int foodGain = (rand() % 5 ) + 1;
	int beerGain = (rand() % 5 ) + 1;

	int incomingDamage = 0; 
	int defendingDamage = 0;

	vector<string> dataStuffs;
 	vector<string> about;
 	*gamePaused = true;
	
	for(index = 0; index < numGaurds; index++){
		incomingDamage += ((rand() % 3) - 1) + gaurdDamage;
	}
	incomingDamage *= 2;
	for(index = 0; index < mercs.size(); index++){
		defendingDamage += mercs[index].calcDamage();
	}

	incomingDamage -= defendingDamage;

	Manager::reportStats();
	cout << "Incoming damage: " + to_string(incomingDamage) << endl;
	

	for(index = 0; index < incomingDamage; index++){
		int i = Manager::getRandomAliveMercIndex();
		if(i >= 0){
			if(mercs[i].currHealth == 0){
				mercs[i].dead = true;
			}else{
				mercs[i].currHealth--;
			}
		}
	}

	if(Manager::getRandomAliveMercIndex() < 0){
		string aboutString = "The merchant demolished your team!";
		about.push_back(aboutString);
	    fpReturnTavern = returnTavern;
	    fpRestartTrail = restartTrail;
	    option resumeOpt = {"Restart from trail", fpRestartTrail, true};
	    option resume2Opt = {"Restart from tavern", fpReturnTavern, true};
	    vector<option> options;
	    options.push_back(resumeOpt);
	    options.push_back(resume2Opt);
	    //Set the data
	    //input loaded skilledMerchantMenu HERE!!!!!!!!! TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    menu->setData("Skilled merchant", about, options, &skilledMerchantMenu, 12, about);
	}else{
		//robberyMenu HERE!!!!!!!!!!!
		gold += goldGain;
		dataStuffs.push_back(to_string(goldGain));
		string aboutString = "You reaped " + to_string(goldGain) + "gold";
		about.push_back(aboutString);
		about.push_back("Hopefully it was worth it");
		about.push_back("and you can sleep at night!");

	    fpContinue = continueGame;
	    option resumeOpt = {"Continue On", fpContinue, true};
	    vector<option> options;
	    options.push_back(resumeOpt);
	    //Set the data
	    menu->setData("Rad robbery", about, options, &merchantRobbedMenu, 8, dataStuffs);
	}
		
	Manager::reportStats();
		
}

void Manager::tickHungerHealth(){
	int i;
	for(i = 0; i < mercs.size(); i++){
		mercs[i].currHunger = (mercs[i].currHunger - 1 > 0) ? mercs[i].currHunger - 1 : 0;
		mercs[i].currHappiness = (mercs[i].currHappiness - 1 > 0) ? mercs[i].currHappiness - 1 : 0;
	}
	Manager::reportStats();
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
		audio->playSoundEffect(MEAT_SOUND);
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
		audio->playSoundEffect(BEER_SOUND);
	}
	else
	{
		cout << "Insufficient Funds" << endl;
	}
}

void Manager::buyMercenary(int mercenaryID, Tavern* tav)
{
	if(tav->tavernCharacters.size() > mercenaryID && tav->tavernCharacters[mercenaryID].cost <= gold)
	{
		cout << "Buying " + tav->tavernCharacters[mercenaryID].firstName + "\n";
		mercs.push_back(tav->tavernCharacters[mercenaryID]);
		gold -= tav->tavernCharacters[mercenaryID].cost;
		tav->tavernCharacters.erase(tav->tavernCharacters.begin() + mercenaryID);
		audio->playSoundEffect(COIN_SOUND);
	}
	reportStats();
}

void Manager::buyMercenaryTrail(int cost)
{
	if(rand() % 10 == 1){
			gold -= cost;
			vector<string> about;
			string aboutString = "He ran away with your gold!";
			about.push_back(aboutString);
		    fpContinue = continueGame;
		    option resumeOpt = {"Onward", fpContinue, true};
		    vector<option> options;
		    options.push_back(resumeOpt);
		    //Set the data
		    menu->setData("Oops", about, options, &oopsRanMenu, 6, about);
		
	}else{
		if(gold >= cost){
			mercs.push_back(*(new Mercenary()));
			gold -= cost;
		}
	}

	reportStats();
}

void Manager::drawMenuManager(){
	menu->drawMenu();
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

SoundPlayer* Manager::getAudio()
{
	return audio;
}

FadeSystem* Manager::getFade()
{
	return fadeSystem;
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

void Manager::feedMerc()
{	
	int i;
	if(food > 0){
		food--;
		for(i = 0; i < mercs.size(); i++){
			Mercenary *m = &mercs[i];
			m->currHunger = (((m->maxHunger / 4) + m->currHunger) >= m->maxHunger) ? m->maxHunger : floor(m->currHunger + (m->maxHunger / 4.0));
		}
	}
}

void Manager::beerMerc()
{
	int i;
	if(beer > 0){
		beer--;
		for(i = 0; i < mercs.size(); i++){
			Mercenary *m = &mercs[i];
			m->currHappiness = (((m->maxHappiness / 4) + m->currHappiness) >= m->maxHappiness) ? m->maxHappiness : floor(m->currHappiness + (m->maxHappiness / 4.0));
		}
	}
}

void Manager::lowerDamage(int index)
{
	mercs[index].currDamage -= floor((float)(mercs[index].currDamage / 2.0));
	mercs[index].sick = true;
}

void Manager::healSickness(int index)
{
	mercs[index].currDamage = mercs[index].maxDamage;
	mercs[index].currHealth = mercs[index].maxHealth;
	mercs[index].currHunger = mercs[index].maxHunger;
	mercs[index].sick = false;
}

int Manager::reportTotalDamage() {
   int damage = 0, i;
   for (i = 0; i < mercs.size(); i++)
	{
	    damage += mercs[i].calcDamage();
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

bool Manager::getInMenu(){
    if(menu->inMenu){
    	printf("In mesnu is true!\n");
    }else{
    	printf("In mesnu is false!\n");
    }
	return menu->inMenu;
}


