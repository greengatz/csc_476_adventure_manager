#include "tavern.h"

#define CUBE 0
#define DOOR 1
#define BOOKSHELF 2
#define STOOL 3
#define RECT_TABLE 4
#define CIR_TABLE 5
#define TABLEWARE 6
#define MUG 7
#define BOTTLE 8
#define TORCH 9
#define BARREL 10

#define NUMFILES 11

const string objFiles[] = {"assets/tavern/cube.obj",
                       	   "assets/tavern/door.obj",
                           "assets/tavern/bookshelf.obj",
                           "assets/tavern/stool.obj",
                           "assets/tavern/table.obj",
                           "assets/tavern/diningtable.obj",
                       	   "assets/tavern/tablware.obj",
                       	   "assets/tavern/mug.obj",
                       	   "assets/tavern/bottle.obj",
                       	   "assets/tavern/torch.obj",
                           "assets/tavern/barrel.obj"};

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
	addTavernItem(STOOL, glm::vec3(0.35, 0.35, 0.35), glm::vec3(initLoc.x+0.5, initLoc.y + 0.2, initLoc.z-0.5), glm::mat4(1.0f));
}

void Tavern::loadTavernMeshes()
{
	float ang;
	glm::mat4 rot;
	for (int iter = 0; iter < 6; iter++) {
		addTavernMesh(objFiles[iter], false);
	}
	//tavern house
	addTavernItem(CUBE, glm::vec3(10.0, 10.0, 10.0), glm::vec3(2000, 1, -20), glm::mat4(1.0f));
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

	createTable1(glm::vec3(25, 0, -25), 0);
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