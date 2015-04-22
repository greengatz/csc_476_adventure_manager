#include "Materials.h"

Materials::Materials()
{

}

void Materials::init(GLuint* newPid, GLint *newh_ka, GLint *newh_kd, GLint *newh_ks, GLint *newh_s)
{
	pid = newPid;
	h_ka = newh_ka;
	h_kd = newh_kd;
	h_ks = newh_ks;
	h_s = newh_s;
}

void Materials::setMaterial(int i)
{
	glUseProgram(*pid);
	switch(i) {
		case 0: //Red Default
			glUniform3f(*h_ka, 0.15, 0.15, 0.15);
			glUniform3f(*h_kd, 0.8, 0.2, 0.2);
			glUniform3f(*h_ks, 0.2, 0.2, 0.2);
			glUniform1f(*h_s, 50.0);
			break;
		case 1: //Green
			glUniform3f(*h_ka, 0.15, 0.15, 0.15);
			glUniform3f(*h_kd, 0.2, 0.8, 0.2);
			glUniform3f(*h_ks, 0.2, 0.2, 0.2);
			glUniform1f(*h_s, 50.0);
			break;
		case 2: //Default light
			glUniform3f(*h_ka, 0.15, 0.15, 0.15);
			glUniform3f(*h_kd, 0.3, 0.3, 0.3);
			glUniform3f(*h_ks, 0.2, 0.2, 0.2);
			glUniform1f(*h_s, 50.0);
			break;
		case 3: //Wood
			glUniform3f(*h_ka, 0.35, 0.35, 0.35);
			glUniform3f(*h_kd, 0.804, 0.666, 0.49);
			glUniform3f(*h_ks, 0.1, 0.1, 0.1);
			glUniform1f(*h_s, 0.01);
			break;
		case 4: //not specular Wood
			glUniform3f(*h_ka, 0.1, 0.1, 0.1);
			glUniform3f(*h_kd, 0.404, 0.366, 0.29);
			glUniform3f(*h_ks, 0.051, 0.051, 0.051);
			glUniform1f(*h_s, 0.01);
			break;
	}
}