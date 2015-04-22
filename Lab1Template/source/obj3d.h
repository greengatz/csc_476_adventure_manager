#ifndef OBJ3D_H
#define OBJ3D_H

#include "GLSL.h"
#include "bounding.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "obj3dcontainer.h"

// #include <glew.h>
// #include <glfw3.h>

using namespace std;
using namespace glm;

class Obj3d
{
	public:
		int num; //this is 
		Obj3dContainer *cont;
		vec3 pos;
		vec3 dir;
		float vel;
		BoundingBox bound;
		bool deadlock;
		bool done;
		int material;
		float locTime;
		vec3 scale;
		mat4 rot;
		GLuint texBuf;
		bool hasTexture;
		int textureNdx;
		int materialNdx;
		Obj3d(Obj3dContainer *newCont, vec3 newScale, int newMaterial, vec3 initPos, mat4 newRot);
		void draw(GLint h_uModelMatrix);
		void move(float deltaTime);
      	void reverseDir();
      	void hit();
		vec3 getCurSpot();
		void loadTextureCoor(int ndx);
		void chooseMaterial(int ndx);
};

#endif
