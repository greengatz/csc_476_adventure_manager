//
// sueda
// November, 2014
//

#include "Camera.h"
#include "MatrixStack.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"

using namespace std;

//Global variables for now ......
//Camera
// horizontal angle : toward -Z


Camera::Camera() :
	aspect(1.0f),
	fovy(45.0f),
	znear(0.1f),
	zfar(1000.0f),
	rotations(0.01, 0.01),
	rfactor(0.0005f),
	pfactor(0.0005f),
	position(25.0f, 0.0f, -25.0f),
	sfactor(0.005f),
	state(0),
	//Brandon Stuff
	lookAtPoint(0.0f, 0.0f, 0.0f),
	theEye(0.0f, 0.0f, 0.0f),
	theStrafe(25.0f, 1.0f, 0.0f),
	theZoom(0.0f, 1.0f, -25.0f),
	freeRoam(false),
	tavernView(true)

{
	bound.createBounds(vec2(-0.6, 0.6), vec2(0, 1.0), vec2(-0.6, 0.6));
}

Camera::~Camera()
{
	
}

glm::vec3 Camera::getLookAtPoint()
{
	return lookAtPoint;
}

glm::vec3 Camera::getTheEye()
{
	return theEye;
}

glm::vec3 Camera::getPosition()
{
	return theStrafe + theZoom;
}

void Camera::setPosition(glm::vec3 aPos)
{
	theStrafe = glm::vec3(aPos.x/2.0, aPos.y/2.0, aPos.z/2.0);
	theZoom = glm::vec3(aPos.x/2.0, aPos.y/2.0, aPos.z/2.0);
}

bool Camera::isTavernView()
{
	return tavernView;
}

bool Camera::isFreeRoam()
{
	return freeRoam;
}

void Camera::toggleGameViews()
{
	tavernView = !tavernView;
	if (tavernView)
	{
		//When going back to tavern, set position at door.
		theStrafe = glm::vec3(23.15, 1.0, 5.82);
		theZoom = glm::vec3(-14.51, 1.0, -27.94);
	}

	freeRoam = false;
}

void Camera::toggleFreeRoam()
{
	if (!tavernView)
	{
		toggleGameViews();
	}
	freeRoam = !freeRoam;
}

void Camera::updateStrafe(glm::vec3 dStrafe)
{
	//free camera
	if (freeRoam)
	{
		theStrafe += dStrafe;
	}
	else
	{
		theStrafe.x += dStrafe.x * 0.1;
		theStrafe.z += dStrafe.z * 0.1;
	}
}

void Camera::updateZoom(glm::vec3 dZoom)
{
	//free camera
	if (freeRoam)
	{
		theZoom += dZoom;
	}
	else
	{
		theZoom.x += dZoom.x * 0.1;
		theZoom.z += dZoom.z * 0.1;
	}
}

void Camera::mouseMoved(int x, int y)
{
	glm::vec2 mouseCurr(x, y);
	rotations += rfactor * mouseCurr;
}

void Camera::setWindowSize(float w, float h)
{
	aspect = w/h;
}

//void Camera::update(const bool *keys, const glm::vec2 &mouse)
void Camera::update(double xpos, double ypos, glm::vec3 wagonPos)
{
	static double xOld = 0;
  	static double yOld = 0;

  	// Compute new orientation
  	horizontalAngle += (mouseSpeed * (xpos - xOld));
  	verticalAngle -= (mouseSpeed * (ypos - yOld));
  	xOld = xpos;
  	yOld = ypos;

  	if (tavernView)
  	{
  		if (verticalAngle > (80.0 * (3.14f)/180.0))
    		verticalAngle = (80.0 * (3.14f)/180.0);
  		else if (verticalAngle < -(80.0 * (3.14f)/180.0))
    		verticalAngle = -(80.0 * (3.14f)/180.0);

    	//Updated look at point for pitch and yaw.
  		lookAtPoint.x = 1.0 * cos(verticalAngle) * cos(horizontalAngle);
  		lookAtPoint.y = 1.0 * sin(verticalAngle);
  		lookAtPoint.z = 1.0 * cos(verticalAngle) * cos(90.0 - horizontalAngle);
   }
   else
   {
   	if (verticalAngle > -(5.0 * (3.14f)/180.0))
    		verticalAngle = -(5.0 * (3.14f)/180.0);
  		else if (verticalAngle < -(20.0 * (3.14f)/180.0))
    		verticalAngle = -(20.0 * (3.14f)/180.0);
  		/*if (horizontalAngle > (45 * (3.14f)/180.0))
    		horizontalAngle = (45 * (3.14f)/180.0);
 		else if (horizontalAngle < -(45 * (3.14f/180.0)))
    		horizontalAngle = -(45 * (3.14f/180.0));*/

  		lookAtPoint.x = 2.0 * -sinf(horizontalAngle) + cosf(verticalAngle);
  		lookAtPoint.y = 2.0 * -sinf(verticalAngle);
  		lookAtPoint.z = 2.0 * cosf(horizontalAngle) + cosf(90.0 - horizontalAngle);
  		lookAtPoint += wagonPos;
   }
}

void Camera::applyProjectionMatrix(MatrixStack *P) const
{
	P->perspective(fovy, aspect, znear, zfar);
}

void Camera::applyViewMatrix(MatrixStack *MV, glm::vec3 wagonPos) const
{
	if (tavernView)
	{
		glm::mat4 View = glm::lookAt((theEye + theStrafe + theZoom), 
			(lookAtPoint + theStrafe + theZoom), glm::vec3(0, 1, 0));
		MV->multMatrix(View);
	}
	else
	{
		glm::mat4 View = glm::lookAt(lookAtPoint, wagonPos, glm::vec3(0, 1, 0));
		MV->multMatrix(View);
	}
  	
}