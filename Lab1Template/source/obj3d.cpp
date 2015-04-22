#include "obj3d.h"

Obj3d::Obj3d(Obj3dContainer *newCont, vec3 newScale, int newMaterial, vec3 initPos, mat4 newRot)
{
  locTime = 0;
  deadlock = false;
  done = false;
  vel = 0.15;
  pos = initPos;
  scale = newScale;
  cont = newCont;
  material = newMaterial;
  rot = newRot;
  bound.createBounds((*cont).shape);
}

/* helper function to make sure your matrix handle is correct */
inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void Obj3d::draw(GLint h_uModelMatrix)
{
  glm::mat4 trans = glm::translate(mat4(1.0f), pos);
  mat4 result = trans * rot * glm::scale(mat4(1.0f), scale);
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(result));
  int nIndices = (int)((*cont).shape[0].mesh.indices.size());
  glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
}

void Obj3d::move(float deltaTime)
{
   locTime += deltaTime * vel;
}

void Obj3d::reverseDir()
{
   vel *= -1.0;
}

void Obj3d::hit()
{
  material = 5;
  done = true;
}

vec3 Obj3d::getCurSpot()
{
  return pos;
}
