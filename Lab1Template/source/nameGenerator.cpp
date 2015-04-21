#include <string>

using namespace std;

string First[] = {"Ricky", "Zdislav"};
int firstCount = 2;

string Last[] = {"Bobby", "Andrzejewski", "Zdrojewski"};
int lastCount = 3;

string Title[] = {"Driver", "Bear-Hugger", "God"};
int titleCount = 2;

string firstName() {
	return First[rand() % firstCount];
}

string lastName() {
	return Last[rand() % lastCount];
}

string title() {
	return Title[rand() % titleCount];
}
