#include "FireSystem.h"

const glm::vec3 fireLoc[] = {glm::vec3(23.0, 0, -23.0), //center fireplace
							 glm::vec3(10.0, 0, -10.0), 
};

FireSystem::FireSystem()
{

}

void FireSystem::init()
{

}

void FireSystem::draw()
{
	for(int iter = 0; iter < firePlace.size(); iter++) {
		firePlace[iter].update();
		firePlace[iter].draw();
	}
}