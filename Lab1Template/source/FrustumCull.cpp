#include "FrustumCull.h"

FrustumCull::FrustumCull()
{

}

FrustumCull::~FrustumCull()
{

}

void FrustumCull::init()
{
	toggled = false;
	hold = false;
	center = vec3(0, 0, 0);
	radius = 0;
}

void FrustumCull::toggleMode()
{
	toggled = !toggled;
	printf("toggled frustum culling to %d\n", toggled);
}

void FrustumCull::holdView()
{
	hold = !hold;
	printf("toggled hold to %d\n", hold);
}

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

	// for (int iter = 0; iter < 4; iter++) {

	// }
	// curProjMat = newProjMat;
	// float minX, maxX, minY, maxY, minZ, maxZ;
	// vec4 transVert = vec4(0.0, 0.0, 0.0, 0.0);
	// //calculates the transformed vertex
	// for (int iter = 0; iter < 4; iter++)
	// {
	// 	transVert.x += projMatIterator[iter * 4] * vert[iter]; 
	// 	transVert.y += projMatIterator[iter * 4 + 1] * vert[iter];
	// 	transVert.z += projMatIterator[iter * 4 + 2] * vert[iter];
	// 	transVert.w += projMatIterator[iter * 4 + 3] * vert[iter];
	// }
	// printf("transVert is %f %f %f %f\n", transVert.x, transVert.y, transVert.z, transVert.w);
}

//returns true if good, false if object should be culled
bool FrustumCull::checkCull(Obj3d target)
{
	// float test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	// projMat = make_mat4(test);

	vec4 vert = vec4(target.pos.x, target.pos.y, target.pos.z, 1.0);

	mat4 dummy = mat4(1.0f);
	const float* projMatIterator = (const float*)value_ptr(dummy);//curProjMat);

	//if turned off
	if (!toggled)
	{
		return true;
	}

	//calculate the Obj3d's current bounding sphere
	target.bound.calcSphere(target.scale, target.pos);

	bool draw = true;
	for (int iter = 0; draw && iter < 6; iter++) {
		if(checkPlane(&projMatIterator, vert, iter / 2, iter % 2, target.bound.center, target.bound.radius)) {
			draw = false;
		}
	}

	if (!draw) {
		printf("would have checked distance between sphere and the target but... not sure how\n");
	}
	return draw;
}

//sign: 0 means (-) and 1 means (+)
//returns true yes to culling
bool FrustumCull::checkPlane(const float** matIter, vec4 vert, int row, int sign, vec3 center, float radius) //may not need these last 2 parameters
{
	float result = 0;
	vec4 planeCoef = vec4(0.0, 0.0, 0.0, 0.0);
	for (int iter = 0; iter < 4; iter++)
	{
		planeCoef[iter] = curProjMat[iter * 4 + 3] + sign * curProjMat[iter * 4 + row];
		// planeCoef[iter] = (*matIter)[12 + iter] + sign * (*matIter)[row * 4 + iter];
		// result += (*matIter)[12 + iter] + sign * (*matIter)[row * 4 + iter];
		// result *= vert[iter];
	}

	//normalize
	float magn = sqrt(planeCoef.x * planeCoef.x + planeCoef.y * planeCoef.y + planeCoef.z * planeCoef.z);
	planeCoef.x /= magn;
	planeCoef.y /= magn;
	planeCoef.z /= magn;
	planeCoef.w /= magn;

	result = planeCoef.x * vert.x + planeCoef.y * vert.y + planeCoef.z * vert.z + planeCoef.w;

	// 0 < result means that its good

	//check dist to plane
	// if (0 >= result)
	// {

	// 	//normalize the plane
	// 	float magn = sqrt(planeCoef.x * planeCoef.x + planeCoef.y * planeCoef.y + planeCoef.z * planeCoef.z);
	// 	planeCoef.x /= magn;
	// 	planeCoef.y /= magn;
	// 	planeCoef.z /= magn;
	// 	planeCoef.w /= magn;

	// 	//
	// 	float dist = planeCoef.x * center.x + planeCoef.y * center.y + planeCoef.z * center.z + planeCoef.w;
	// 	return distance > radius;
	// }

	return result < 0;
}