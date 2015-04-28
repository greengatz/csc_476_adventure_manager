#include "obj3dcontainer.h"

//Given a vector of shapes which has already been read from an obj file
// resize all vertices to the range [-1, 1]
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

            if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

            if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
        }
    }
   //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
     maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
     maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
     maxExtent = zExtent;
   }
    scaleX = 2.0 /maxExtent;
    shiftX = minX + (xExtent/ 2.0);
    scaleY = 2.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 2.0/ maxExtent;
    shiftZ = minZ + (zExtent)/2.0;

    //Go through all verticies shift and scale them
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
            assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
            assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
            assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
        }
    }
}

//loads a tinyobj
void Obj3dContainer::loadIntoTinyOBJ(const string name) {
  string err = tinyobj::LoadObj(shape, material, name.c_str());
  if(!err.empty()) {
    std::cerr << err << endl;
  }
  resize_obj(shape);
}

//loads a buffer of floats
void loadBuff(vector<float> buff, GLuint *objBuff) {
  glGenBuffers(1, &(*objBuff));
  glBindBuffer(GL_ARRAY_BUFFER, *objBuff);
  glBufferData(GL_ARRAY_BUFFER, (buff).size()*sizeof(float), &(buff)[0], GL_STATIC_DRAW);
}

//loads a buffer of ints
void loadBuff(vector<unsigned int> buff, GLuint *objBuff) {
  glGenBuffers(1, &(*objBuff));
  glBindBuffer(GL_ARRAY_BUFFER, *objBuff);
  glBufferData(GL_ARRAY_BUFFER, (buff).size()*sizeof(float), &(buff)[0], GL_STATIC_DRAW);
}

void loadObjNorms(vector<tinyobj::shape_t> objShape, vector<float> *norObjBuf, GLuint *objBuff) {
   int idx1, idx2, idx3; //the vertex's index
   glm::vec3 v1, v2, v3; //the vertexes
   //for every vertex initialize a normal to 0
   for (int j = 0; j < objShape[0].mesh.positions.size()/3; j++) {
      (*norObjBuf).push_back(0);
      (*norObjBuf).push_back(0);
      (*norObjBuf).push_back(0);
   }

   for (int i = 0; i < objShape[0].mesh.indices.size()/3; i++) {
      idx1 = objShape[0].mesh.indices[3*i+0];
      idx2 = objShape[0].mesh.indices[3*i+1];
      idx3 = objShape[0].mesh.indices[3*i+2];
      v1 = glm::vec3(objShape[0].mesh.positions[3*idx1 +0],objShape[0].mesh.positions[3*idx1 +1], objShape[0].mesh.positions[3*idx1 +2]); 
      v2 = glm::vec3(objShape[0].mesh.positions[3*idx2 +0],objShape[0].mesh.positions[3*idx2 +1], objShape[0].mesh.positions[3*idx2 +2]); 
      v3 = glm::vec3(objShape[0].mesh.positions[3*idx3 +0],objShape[0].mesh.positions[3*idx3 +1], objShape[0].mesh.positions[3*idx3 +2]);

      //normal calculation
      glm::vec3 edge1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
      glm::vec3 edge2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
      glm::vec3 norm = glm::normalize(glm::cross(edge1, edge2));

      (*norObjBuf)[3*idx1+0] += norm.x; 
      (*norObjBuf)[3*idx1+1] += norm.y; 
      (*norObjBuf)[3*idx1+2] += norm.z; 
      (*norObjBuf)[3*idx2+0] += norm.x; 
      (*norObjBuf)[3*idx2+1] += norm.y; 
      (*norObjBuf)[3*idx2+2] += norm.z; 
      (*norObjBuf)[3*idx3+0] += norm.x; 
      (*norObjBuf)[3*idx3+1] += norm.y; 
      (*norObjBuf)[3*idx3+2] += norm.z; 
   }

   loadBuff(*norObjBuf, &(*objBuff));
}

Obj3dContainer::Obj3dContainer()
{
  
}

Obj3dContainer::Obj3dContainer(const string name)
{
  loadIntoTinyOBJ(name);
}

//noNorms is true if the obj files has not norms already in it
void Obj3dContainer::initBuffers(bool noNorms) 
{
	const vector<float> &shapePos = shape[0].mesh.positions;
	loadBuff(shapePos, &posBuf);
  
  vector<float> shapeNor;
  if (noNorms)
  {
    loadObjNorms(shape, &shapeNor, &norBuf);
  }
  else
  {
    const vector<float> &shapeNorBuff = shape[0].mesh.normals;
    loadBuff(shapeNorBuff, &norBuf);
    shapeNor = shapeNorBuff;
  }

	const vector<unsigned int> &shapeInd = shape[0].mesh.indices;
	loadBuff(shapeInd, &indBuf);
}

int Obj3dContainer::getIndices() 
{
	return (int)shape[0].mesh.indices.size();
}