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
		Obj3dContainer *cont;
		vec3 pos;
		vec3 dir; //currently not used
		float vel; //currently not used
		BoundingBox bound;
		float locTime;
		vec3 scale;
		mat4 rot;
        mat4 preTrans;
        mat4 moveRot;
		GLuint texBuf;
		//texture and material data
		bool hasTexture;
		int textureNdx;
		int materialNdx;

		Obj3d(Obj3dContainer *newCont, vec3 newScale, vec3 initPos, mat4 newRot);
		void draw(GLint h_uModelMatrix);
		vec3 getCurSpot();
		void loadTextureCoor(int ndx);
		void chooseMaterial(int ndx);
};

#endif
