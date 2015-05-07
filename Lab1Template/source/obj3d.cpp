#include "obj3d.h"

Obj3d::Obj3d(Obj3dContainer *newCont, vec3 newScale, vec3 initPos, mat4 newRot)
{
  locTime = 0;
  vel = 0;
  pos = initPos;
  scale = newScale;
  cont = newCont;
  rot = newRot;
  bound.createBounds((*cont).shape);
  hasTexture = false;
  materialNdx = -1;
  preTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
  moveRot = glm::mat4(1.0f);
}

/* helper function to make sure your matrix handle is correct */
inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void Obj3d::draw(GLint h_uModelMatrix)
{
  glm::mat4 trans = glm::translate(mat4(1.0f), pos);
  mat4 result = moveRot * trans * rot * preTrans * glm::scale(mat4(1.0f), scale);
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(result));
  int nIndices = (int)((*cont).shape[0].mesh.indices.size());
  glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
}

vec3 Obj3d::getCurSpot()
{
  return pos;
}

void Obj3d::loadTextureCoor(int ndx)
{
   const vector<float> &shapeTexBuff = (*cont).shape[0].mesh.texcoords;
   glGenBuffers(1, &texBuf);
   glBindBuffer(GL_ARRAY_BUFFER, texBuf);
   glBufferData(GL_ARRAY_BUFFER, shapeTexBuff.size()*sizeof(float), &shapeTexBuff[0], GL_STATIC_DRAW);
   hasTexture = true;
   textureNdx = ndx;
}

void Obj3d::chooseMaterial(int ndx)
{
  materialNdx = ndx;
}
