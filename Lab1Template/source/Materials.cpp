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
		case 5: //supposedly red
			glUniform3f(*h_ka, 0.17, 0.11, 0.11);
			glUniform3f(*h_kd, 0.604, 0.04, 0.04);
			glUniform3f(*h_ks, 0.076, 0.051, 0.051);
			glUniform1f(*h_s, 0.01);
			break;
		case 6: //dark brown
			glUniform3f(*h_ka, 0.185, 0.1, 0.045);
			glUniform3f(*h_kd, 0.361, 0.2, 0.09);
			glUniform3f(*h_ks, 0.076, 0.051, 0.051);
			glUniform1f(*h_s, 0.01);
			break;
		case 7: //grey
			glUniform3f(*h_ka, 0.1, 0.1, 0.1);
			glUniform3f(*h_kd, 0.4, 0.4, 0.04);
			glUniform3f(*h_ks, 0.015, 0.015, 0.015);
			glUniform1f(*h_s, 0.01);
			break;
		case 8: //white
			glUniform3f(*h_ka, 0.1, 0.1, 0.1);
			glUniform3f(*h_kd, 1.0, 1.0, 1.0);
			glUniform3f(*h_ks, 0.05, 0.05, 0.05);
			glUniform1f(*h_s, 0.01);
			break;
		case 9: //default color
			glUniform3f(*h_ka, 0.25, 0.25, 0.25);
			glUniform3f(*h_kd, 0.4, 0.4, 0.4);
			glUniform3f(*h_ks, 0.01, 0.01, 0.01);
			glUniform1f(*h_s, 0.00001);
			break;
		case 10: //dark brown
			glUniform3f(*h_ka, 0.185, 0.1, 0.045);
			glUniform3f(*h_kd, 0.392, 0.2549, 0.0902);
			glUniform3f(*h_ks, 0.076, 0.051, 0.051);
			glUniform1f(*h_s, 0.01);
			break;
		case 11: //very dark brown
			glUniform3f(*h_ka, 0.185, 0.1, 0.045);
			glUniform3f(*h_kd, 0.2, 0.098, 0.0);
			glUniform3f(*h_ks, 0.076, 0.051, 0.051);
			glUniform1f(*h_s, 0.01);
			break;
		case 12: //rope
			glUniform3f(*h_ka, 0.185, 0.1, 0.045);
			glUniform3f(*h_kd, 0.55294, 0.31765, 0.094118);
			glUniform3f(*h_ks, 0.55294, 0.31765, 0.094118);
			glUniform1f(*h_s, 0.01);
			break;
		case 13: //not specular
			glUniform3f(*h_ka, 0.1, 0.1, 0.1);
			glUniform3f(*h_kd, 0.1, 0.1, 0.01);
			glUniform3f(*h_ks, 0.00015, 0.00015, 0.00015);
			glUniform1f(*h_s, 0.001);
			break;
		case 14: //not specular
			glUniform3f(*h_ka, 0.085, 0.01, 0.0045);
			glUniform3f(*h_kd, 0.627, 0.3215, 0.1764);
			glUniform3f(*h_ks, 0.076, 0.051, 0.051);
			glUniform1f(*h_s, 0.001);
			break;
	}
}