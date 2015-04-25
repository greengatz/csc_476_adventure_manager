#include "obj3d.h"
#include "bounding.h"
using namespace std;
using namespace glm;

//normal creation of bounds
void BoundingBox::createBounds(vector<tinyobj::shape_t> objShape) {
  minX = maxX = objShape[0].mesh.positions[objShape[0].mesh.indices[0]];
  minY = maxY = objShape[0].mesh.positions[objShape[0].mesh.indices[1]];
  minZ = maxZ = objShape[0].mesh.positions[objShape[0].mesh.indices[2]];
  for (int iter = 1; iter < objShape[0].mesh.indices.size(); iter++) {
    
    int idx = objShape[0].mesh.indices[iter];
    minX = (minX < objShape[0].mesh.positions[3*idx+0]) ? minX : objShape[0].mesh.positions[3*idx+0];
    maxX = (maxX > objShape[0].mesh.positions[3*idx+0]) ? maxX : objShape[0].mesh.positions[3*idx+0];
    minY = (minY < objShape[0].mesh.positions[3*idx+1]) ? minY : objShape[0].mesh.positions[3*idx+1];
    maxY = (maxY > objShape[0].mesh.positions[3*idx+1]) ? maxY : objShape[0].mesh.positions[3*idx+1];
    minZ = (minZ < objShape[0].mesh.positions[3*idx+2]) ? minZ : objShape[0].mesh.positions[3*idx+2];
    maxZ = (maxZ > objShape[0].mesh.positions[3*idx+2]) ? maxZ : objShape[0].mesh.positions[3*idx+2];
  }
}

//used for setting size of camera
void BoundingBox::createBounds(vec2 xSize, vec2 ySize, vec2 zSize) {
  minX = xSize.x;
  maxX = xSize.y;
  minY = ySize.x;
  maxY = ySize.y;
  minZ = zSize.x;
  maxZ = zSize.y;
}

void BoundingBox::calcSphere(vec3 scale, vec3 pos)
{
  float box[6] = {
    minX * scale.x + pos.x,
    maxX * scale.x + pos.x,
    minY * scale.y + pos.y,
    maxY * scale.y + pos.y,
    minZ * scale.z + pos.z,
    maxZ * scale.z + pos.z,
  };

  center.x = (box[1] - box[0]) / 2.0 + box[0];
  center.y = (box[3] - box[2]) / 2.0 + box[2];
  center.z = (box[5] - box[4]) / 2.0 + box[4];
  radius = glm::distance(center, glm::vec3(box[0], box[2], box[4]));
}

void BoundingBox::calcSphere(vec3 newCenter, float newRad) 
{
  center = newCenter;
  radius = newRad;
}

bool BoundingBox::checkCollision(float cam[], vec3 scale, vec3 trans)
{
  bool collide = false;

  float box[6] = {
    minX * scale.x + trans.x,
    maxX * scale.x + trans.x,
    1,
    1,
    minZ * scale.z + trans.z,
    maxZ * scale.z + trans.z,
  };

  if ((box[0] >= cam[0] && box[0] <= cam[1] || 
       box[1] >= cam[0] && box[1] <= cam[1]) &&
      (box[2] >= cam[2] && box[2] <= cam[3] || 
       box[3] >= cam[2] && box[3] <= cam[3]) &&
      (box[4] >= cam[4] && box[4] <= cam[5] || 
       box[5] >= cam[4] && box[5] <= cam[5])) {
    collide = true;
  }

	return collide;
}