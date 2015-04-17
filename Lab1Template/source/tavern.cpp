#include "tavern.h"

Tavern::Tavern()//GLint *pos, GLint *nor)
{
	// h_vertPos = *pos;
	// h_vertNor = *nor;
}

void Tavern::addTavernMesh(const string filename)
{
	Obj3dContainer temp;
	temp.loadIntoTinyOBJ(filename);
	temp.initBuffers();
	tavernMeshes.push_back(temp);
}

//index is the index of buffer info in tavernMeshes
void Tavern::addTavernItem(int index, glm::vec3 scale, glm::vec3 trans)
{
	Obj3d temp(&(tavernMeshes[index]), scale, 0, trans);
	tavernItems.push_back(temp);
}

void Tavern::loadTavernMeshes()
{
	addTavernMesh("assets/cube.obj");
	addTavernItem(0, glm::vec3(.15, .35, .15), glm::vec3(0, .5, 0));
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
	enableBuff(h_vertPos, h_vertNor, tavernMeshes[0].posBuf, tavernMeshes[0].norBuf, tavernMeshes[0].indBuf);
	tavernItems[0].draw(h_ModelMatrix, glm::mat4(1.0f));
	disableBuff(h_vertPos, h_vertNor);
}