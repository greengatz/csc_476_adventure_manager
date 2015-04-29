#include "FrustumCull.h"

FrustumCull::FrustumCull()
{

}

FrustumCull::~FrustumCull()
{

}

void FrustumCull::init()
{
	toggled = true;
	hold = false;
}
//turns on and off frustum culling and in the process also turns hold off
void FrustumCull::toggleMode()
{
	toggled = !toggled;
	hold = false;
	printf("toggled frustum culling to %d\n", toggled);
}

//if hold is true than the frustum does not update the latest projection matrix
void FrustumCull::holdView()
{
	hold = !hold;
	printf("toggled hold to %d\n", hold);
}

//calculates the projection matrix of the frustum
void FrustumCull::setProjMat(mat4 projMat, mat4 viewMat)
{
	if (!hold) {
		const float* proj = (const float*)value_ptr(projMat);
		const float* view = (const float*)value_ptr(viewMat);
		float com[16];
		for (int iter = 0; iter < 4; iter++) {
			float temp1 = view[iter * 4];
			float temp2 = view[iter * 4 + 1];
			float temp3 = view[iter * 4 + 2];
			float temp4 = view[iter * 4 + 3];
			curProjMat[iter * 4 + 0] = temp1 * proj[0] + temp2 * proj[4] + temp1 * proj[8] + temp4 * proj[12];
			curProjMat[iter * 4 + 1] = temp1 * proj[1] + temp2 * proj[5] + temp1 * proj[9] + temp4 * proj[13];
			curProjMat[iter * 4 + 2] = temp1 * proj[2] + temp2 * proj[6] + temp1 * proj[10] + temp4 * proj[14];
			curProjMat[iter * 4 + 3] = temp1 * proj[3] + temp2 * proj[7] + temp3 * proj[11] + temp4 * proj[15];
		}
	}
}

//returns true if good, false if object should be culled
bool FrustumCull::checkCull(Obj3d target)
{
	vec4 vert = vec4(target.pos.x, target.pos.y, target.pos.z, 1.0);

	//if turned off
	if (!toggled)
	{
		return true;
	}

	//calculate the Obj3d's current bounding sphere
	target.bound.calcSphere(target.scale, target.pos);

	//tests Obj3d against the 6 planes
	bool draw = true;
	for (int iter = 0; draw && iter < 6; iter++) {
		if(checkPlane(vert, iter / 2, iter % 2, target.bound.center, target.bound.radius)) {
			draw = false;
		}
	}

	return draw;
}

//sign: 0 means (-) and 1 means (+)
//returns true yes to culling
bool FrustumCull::checkPlane(vec4 vert, int row, int sign, vec3 tCenter, float tRadius)
{
	//calculates the plane coefficients
	vec4 planeCoef = vec4(0.0, 0.0, 0.0, 0.0);
	for (int iter = 0; iter < 4; iter++)
	{
		planeCoef[iter] = curProjMat[iter * 4 + 3] + sign * curProjMat[iter * 4 + row];
	}

	float dist = planeCoef.x * tCenter.x + planeCoef.y * tCenter.y + planeCoef.z * tCenter.z + planeCoef.w;
	
	//normalize the magnitude
	float magn = sqrt(planeCoef.x * planeCoef.x + planeCoef.y * planeCoef.y + planeCoef.z * planeCoef.z);
	planeCoef.x /= magn;
	planeCoef.y /= magn;
	planeCoef.z /= magn;
	planeCoef.w /= magn;
	dist /= magn;

	//saves which side of the plane the object iss
	float result = planeCoef.x * vert.x + planeCoef.y * vert.y + planeCoef.z * vert.z + planeCoef.w;	

	//if the center of the object is on the wrong side of the plane make dist to compare it against the radius
	if (result < 0) {
		dist *= -1;
	}

	return result < 0 && dist >= tRadius;
}