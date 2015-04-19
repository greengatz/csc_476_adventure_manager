#include "tavern.h"

#define CUBE 0
#define DOOR 1
#define BOOKSHELF 2
#define STOOL 3
#define CHAIR 4
#define RECT_TABLE 5
#define CIR_TABLE 6
#define MUG 7
#define BOTTLE 8
#define TORCH 9
#define BARREL 10
#define STICK 11
#define POLE 12
#define FIREPLACE 13
#define TABLEWARE 14

#define NUMFILES 12

const string objFiles[] = {"assets/tavern/cube.obj",
                       	   "assets/tavern/door.obj",
                           "assets/tavern/bookshelf.obj",
                           "assets/tavern/stool.obj",
                           "assets/tavern/chair.obj",
                           "assets/tavern/table.obj",
                           "assets/tavern/diningtable.obj",
                       	   "assets/tavern/mug.obj",
                       	   "assets/tavern/bottle.obj",
                       	   "assets/tavern/torch.obj",
                           "assets/tavern/barrel.obj",
                           "assets/tavern/stick.obj",
                           "assets/tavern/pole.obj",
                           "assets/tavern/fireplace.obj",
                           "assets/tavern/tablware.obj"};

Tavern::Tavern()//GLint *pos, GLint *nor)
{
	// h_vertPos = *pos;
	// h_vertNor = *nor;
}

void Tavern::addTavernMesh(const string filename, bool noNorms)
{
	Obj3dContainer temp;
	temp.loadIntoTinyOBJ(filename);
	temp.initBuffers(noNorms);
	tavernMeshes.push_back(temp);
}

//index is the index of buffer info in tavernMeshes
void Tavern::addTavernItem(int index, glm::vec3 scale, glm::vec3 trans, glm::mat4 rot)
{
	Obj3d temp(&(tavernMeshes[index]), scale, 0, trans, rot);
	tavernItems.push_back(temp);
}

void Tavern::createTable1(glm::vec3 initLoc, float ang)
{
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(RECT_TABLE, glm::vec3(1.6, 1.6, 1.6), glm::vec3(initLoc.x, initLoc.y + 0.5, initLoc.z), rot);
	//make chairs face the table
	if (ang == 0)
	{
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x - 1, initLoc.y + 0.5, initLoc.z + 1), rot);
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + 1, initLoc.y + 0.5, initLoc.z + 1), rot);
		ang += 180;
		rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x - 1, initLoc.y + 0.5, initLoc.z - 1), rot);
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + 1, initLoc.y + 0.5, initLoc.z - 1), rot);
	}
	else
	{
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + 1, initLoc.y + 0.5, initLoc.z - 1), rot);
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x + 1, initLoc.y + 0.5, initLoc.z + 1), rot);
		ang += 180;
		rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));	
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x - 1, initLoc.y + 0.5, initLoc.z + 1), rot);
		addTavernItem(CHAIR, glm::vec3(0.75, 0.75, 0.75), glm::vec3(initLoc.x - 1, initLoc.y + 0.5, initLoc.z - 1), rot);
	}
}

void Tavern::loadTavernMeshes()
{
	float ang;
	glm::mat4 rot;
	for (int iter = 0; iter < NUMFILES - 1; iter++) {
		addTavernMesh(objFiles[iter], false);
	}
	addTavernMesh("assets/tavern/stick.obj", true);
	addTavernMesh("assets/tavern/pole.obj", true);
	addTavernMesh("assets/tavern/fireplace.obj", true);

	//tavern house
	addTavernItem(CUBE, glm::vec3(10.0, 10.0, 10.0), glm::vec3(20, 1, -20), glm::mat4(1.0f));
	//counter
	addTavernItem(CUBE, glm::vec3(5.0, 1, 0.5), glm::vec3(15, 0.2, -15), glm::mat4(1.0f));
	addTavernItem(CUBE, glm::vec3(5.0, 0.1, 0.55), glm::vec3(15, 1.1, -15), glm::mat4(1.0f));
	addTavernItem(CUBE, glm::vec3(0.5, 1, 2), glm::vec3(21.9, 0.2, -13.5), glm::mat4(1.0f));
	addTavernItem(CUBE, glm::vec3(0.55, 0.1, 2), glm::vec3(21.9, 1.1, -13.5), glm::mat4(1.0f));
	// addTavernItem(CUBE, glm::vec3(5.0, 0.1, 0.55), glm::vec3(15, 1.1, -15), glm::mat4(1.0f));
	//door
	addTavernItem(DOOR, glm::vec3(1.5, 1.5, 1.5), glm::vec3(5, 1.35, -23), glm::mat4(1.0f));
	//adding bookshelves
	ang = 180;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 1.0f, 0));
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(12.4, 1.75, -12.5), rot);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(13.7, 1.75, -12.5), rot);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(15, 1.75, -12.5), rot);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(16.3, 1.75, -12.5), rot);
	addTavernItem(BOOKSHELF, glm::vec3(1.0, 1.0, 1.0), glm::vec3(17.6, 1.75, -12.5), rot);

	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(11, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(12, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(12.9, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(13.5, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(15, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(16.2, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(17.5, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(18.2, .35, -16), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(18.9, .35, -16), glm::mat4(1.0f));


	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(23, .35, -15.4), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(23, .35, -14.2), glm::mat4(1.0f));
	addTavernItem(STOOL, glm::vec3(0.35, 0.32, 0.35), glm::vec3(23, .35, -13.6), glm::mat4(1.0f));

	createTable1(glm::vec3(13, 0, -25), 0);
	createTable1(glm::vec3(17, 0, -25), 0);
	createTable1(glm::vec3(21, 0, -25), 0);
	createTable1(glm::vec3(13, 0, -29), 0);
	createTable1(glm::vec3(17, 0, -29), 0);
	createTable1(glm::vec3(21, 0, -29), 0);
	createTable1(glm::vec3(13, 0, -33), 0);
	createTable1(glm::vec3(17, 0, -33), 0);
	createTable1(glm::vec3(21, 0, -33), 0);

	createTable1(glm::vec3(28, 0, -19), 90);
	createTable1(glm::vec3(32, 0, -19), 90);
	createTable1(glm::vec3(36, 0, -19), 90);
	createTable1(glm::vec3(28, 0, -23), 90);
	createTable1(glm::vec3(32, 0, -23), 90);
	createTable1(glm::vec3(36, 0, -23), 90);

	//straight up 
	addTavernItem(BARREL, glm::vec3(1.0, 1.2, 1.0), glm::vec3(10.0, 0, -13.0), glm::mat4(1.0f));
	//side barrel
	ang = 90;
	rot = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(1.0f, 0, 0));
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(35.0, 0, -35.0), rot);
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(33.6, 0, -35.0), rot);
	addTavernItem(BARREL, glm::vec3(1.0, 1.0, 1.0), glm::vec3(34.3, 1.25, -35.0), rot);

	//fireplace and roasting
	addTavernItem(FIREPLACE, glm::vec3(0.6, 0.6, 0.6), glm::vec3(30, -.55, -29.7), glm::mat4(1.0f));
	addTavernItem(FIREPLACE, glm::vec3(0.6, 0.6, 0.6), glm::vec3(30, -.55, -30.3), glm::mat4(1.0f));
	addTavernItem(FIREPLACE, glm::vec3(0.6, 0.6, 0.6), glm::vec3(30, -.4, -30), glm::mat4(1.0f));
	addTavernItem(STICK, glm::vec3(1.0, 1.5, 1.0), glm::vec3(29, 0, -30.075), glm::mat4(1.0f));
	addTavernItem(STICK, glm::vec3(1.0, 1.5, 1.0), glm::vec3(31, 0, -30.075), glm::mat4(1.0f));
	addTavernItem(POLE, glm::vec3(1.1, 1.2, 1.2), glm::vec3(30, 1, -30), glm::mat4(1.0f));
}

void Tavern::enableBuff(GLint h_vertPos, GLint h_vertNor, GLuint posBuf, GLuint norBuf, GLuint indBuf) {
  GLSL::enableVertexAttribArray(h_vertPos); //position
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLSL::enableVertexAttribArray(h_vertNor); //normals
  glBindBuffer(GL_ARRAY_BUFFER, norBuf);
  glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
}

void Tavern::disableBuff(GLint h_vertPos, GLint h_vertNor) {
  GLSL::disableVertexAttribArray(h_vertPos);
  GLSL::disableVertexAttribArray(h_vertNor);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Tavern::drawTavern(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor)
{
	for (int iter = 0; iter < tavernItems.size(); iter++) {
		enableBuff(h_vertPos, h_vertNor, (*tavernItems[iter].cont).posBuf, (*tavernItems[iter].cont).norBuf, (*tavernItems[iter].cont).indBuf);
		tavernItems[iter].draw(h_ModelMatrix);
		disableBuff(h_vertPos, h_vertNor);
	}
}