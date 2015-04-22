#include "obj3d.h"
#include "bounding.h"
using namespace std;
using namespace glm;

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

void BoundingBox::createBounds(vec2 xSize, vec2 ySize, vec2 zSize) {
  minX = xSize.x;
  maxX = xSize.y;
  minY = ySize.x;
  maxY = ySize.y;
  minZ = zSize.x;
  maxZ = zSize.y;
}

bool BoundingBox::checkCollision(float cam[], vec3 scale, vec3 trans)
{
  bool collide = false;

  float box[6] = {
    minX * scale.x + trans.x,
    maxX * scale.x + trans.x,
    minY * scale.y + trans.y,
    maxY * scale.y + trans.y,
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