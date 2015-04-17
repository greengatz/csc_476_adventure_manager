//
// sueda
// November, 2014
//

#pragma  once
#ifndef __Camera__
#define __Camera__

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#define xBoundMax 48.5
#define xBoundMin 0.5
#define zBoundMax -0.5
#define zBoundMin -48.5

class MatrixStack;

class Camera
{
public:

	enum {
		ROTATE = 0,
		TRANSLATE,
		SCALE
	};
	
	Camera();
	virtual ~Camera();
	void setWindowSize(float w, float h);
	void update(double xpos, double ypos);
	void mouseMoved(int x, int y);
	void applyProjectionMatrix(MatrixStack *P) const;
	void applyViewMatrix(MatrixStack *MV) const;
	//Brandon stuff
	glm::vec3 getLookAtPoint();
	glm::vec3 getTheEye();
	glm::vec3 getPosition();
	void updateStrafe(glm::vec3 dStrafe);
	void updateZoom(glm::vec3 dZoom);
	
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	float rfactor;
	float pfactor;
	float sfactor;
	int state;
	glm::vec2 mousePrev;
	glm::vec2 rotations;
	glm::vec3 position;

	//Brandon stuff
	glm::vec3 lookAtPoint;
	glm::vec3 theEye;
	glm::vec3 theStrafe;
	glm::vec3 theZoom;
};

#endif
