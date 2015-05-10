#ifndef SPLINE_H
#define SPLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "GLSL.h"
#include "Camera.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "Eigen/Dense"
//#include <armadillo>
/*#include "obj3dcontainer.h"
#include "obj3d.h"
#include "mercenary.h"
#include "tavern.h"*/

using namespace std;
using namespace glm;
using Eigen::MatrixXd;
using Eigen::VectorXd;

class Spline
{
	public:
		Spline(vector<glm::vec2> points, float initSlope, float endSlope);
		Spline(); // don't use this ever
		float getY(float x);
		float getDY(float x);
        void printSpline();

	private:
        class SplineSegment
        {
            public:
                float startX;
                float endX;
                SplineSegment(VectorXd coeff, float start, float end);
                float getY(float x);
                float getDY(float x);
                void printSegment();

            private:
                VectorXd coefficients;
        };

        vector<glm::vec2> input;
		vector<SplineSegment> pieces;
        VectorXd coefficients;
};

#endif
