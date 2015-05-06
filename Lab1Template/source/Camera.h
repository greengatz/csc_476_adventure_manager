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
#include "bounding.h"

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
	void update(double xpos, double ypos, glm::vec3 wagonPos);
	void mouseMoved(int x, int y);
	void applyProjectionMatrix(MatrixStack *P) const;
	void applyViewMatrix(MatrixStack *MV, glm::vec3 wagonPos) const;
	//Brandon stuff
	glm::vec3 getLookAtPoint();
	glm::vec3 getTheEye();
	glm::vec3 getPosition();
	void updateStrafe(glm::vec3 dStrafe);
	void updateZoom(glm::vec3 dZoom);
	void updateWagonZoom(double yoffset);
	void toggleFreeRoam();
	void setTavernView();
	void setTrailView();
	bool isTavernView();
	bool isFreeRoam();
	void toggleGameViews();
	void setPosition(glm::vec3 aPos);

	BoundingBox bound;
	float horizontalAngle = -3.14f/2.0f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	//Mouse sensitivity
	float mouseSpeed = 0.005f;
	//Strafe speed
	float strafeSpeed = 0.5f;
	//Walk speed
	float walkSpeed = 0.5f;
	//Sprint speed
	float sprintSpeed = 5.0f;
	
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	float rfactor;
	float pfactor;
	float sfactor;
	int state;
	bool freeRoam;
	bool tavernView;
	glm::vec2 mousePrev;
	glm::vec2 rotations;
	glm::vec3 position;
	float wagonZoom;

	//Brandon stuff
	glm::vec3 lookAtPoint;
	glm::vec3 theEye;
	glm::vec3 theWagonEye;
	glm::vec3 theStrafe;
	glm::vec3 theZoom;
};

#endif
