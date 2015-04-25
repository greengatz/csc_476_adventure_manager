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
}

void FrustumCull::toggleMode()
{
	toggled = !toggled;
	printf("toggled frustum culling to %d\n", toggled);
}

void FrustumCull::setProjMat(mat4 newProjMat)
{
	curProjMat = newProjMat;
}

//returns true if good, false if object should be culled
bool FrustumCull::checkCull(Obj3d target)
{
	// float test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	// projMat = make_mat4(test);

	vec4 vert = vec4(target.pos.x, target.pos.y, target.pos.z, 1.0);
	const float* projMatIterator = (const float*)value_ptr(curProjMat);

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

	//if turned off
	if (!toggled)
	{
		return true;
	}

	//calculate the Obj3d's current bounding sphere
	target.bound.calcSphere(target.scale, target.pos);

	bool draw = true;
	for (int iter = 0; draw && iter < 6; iter++) {
		if(!checkPlane(&projMatIterator, vert, iter / 2, iter % 2, target.bound.center, target.bound.radius)) {
			draw = false;
			printf("returning draw as %d\n", draw);
		}
	}
	return draw;
}

//sign: 0 means (-) and 1 means (+)
//returns true if 0 < row4 + row * sign * vert, meaning true means do not cull
bool FrustumCull::checkPlane(const float** matIter, vec4 vert, int row, int sign, vec3 center, float radius)
{
	float result = 0;
	vec4 planeCoef = vec4(0.0, 0.0, 0.0, 0.0);
	for (int iter = 0; iter < 4; iter++)
	{
		planeCoef[iter] = (*matIter)[12 + iter] + sign * (*matIter)[row * 4 + iter];
		result += (*matIter)[12 + iter] + sign * (*matIter)[row * 4 + iter];
		result *= vert[iter];
	}

	// 0 < result means that its good

	//check dist to plane
	if (0 >= result)
	{
		float dist = planeCoef.x * center.x + planeCoef.y * center.y + planeCoef.z * center.z + planeCoef.w;
		dist /= sqrt(planeCoef.x * planeCoef.x + planeCoef.y * planeCoef.y + planeCoef.z * planeCoef.z);
		return radius > dist;
	}

	return 0 < result;
}