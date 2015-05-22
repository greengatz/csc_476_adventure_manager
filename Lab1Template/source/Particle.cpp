#include "Particle.h"

float Particle::randFloat(float low, float high)
{
	float num = rand() / (float)RAND_MAX;
	return (1.0f - num) * low + num * high;
}

Particle::Particle()
{
	mass = 0.01;
	lifespan = 1.0;
	scale = 1.0;
}

bool Particle::isSmokey()
{
	return smoke;
}

void Particle::init(int staggerTime, bool isSmoke)
{
	mass = 0.01;
	smoke = isSmoke;
	stagger = staggerTime;
	rebirth(0.0);
}

void Particle::rebirth(float curTime)
{
	if (!smoke) {
		pos.x = randFloat(-0.2f, 0.2f);
		pos.y = randFloat(-0.15f, 0.15f);
		pos.z = randFloat(-0.2f, 0.2f);
		vel.x = randFloat(-0.001f, 0.001f);
		// vel.y = randFloat(0.0f, 0.005f);
		vel.z = randFloat(-0.001f, 0.001f);
		// vel.x = randFloat(-0.01f, 0.01f);
		// vel.y = randFloat(0.0f, 0.015f);
		vel.y = randFloat(0.03f, 0.0175f);
		// vel.z = randFloat(-0.01f, 0.01f);
		// lifespan = randFloat(0.08f, 0.125f);
		lifespan = randFloat(0.01f, 0.04f);
		endLife = curTime + lifespan;
		// scale = randFloat(0.075f, 0.1f);
		scale = randFloat(0.175f, 0.4f);
		col = glm::vec4(1.0f, randFloat(0.92f, 1.0f), 0, randFloat(0.8f, 1.0f));
		origTrans = col.w;
	}
	else {
		pos.x = randFloat(-0.15f, 0.15f);
		pos.y = randFloat(1.0f, 1.1f);
		pos.z = randFloat(-0.15f, 0.15f);
		vel.x = randFloat(-0.001f, 0.0005f);
		vel.y = randFloat(0.005f, 0.01f);
		vel.z = randFloat(-0.001f, 0.0005f);
		lifespan = randFloat(0.08f, 0.125f);
		endLife = curTime + lifespan;
		// scale = randFloat(0.05f, 0.075f);
		scale = randFloat(0.2f, 0.3f);
		float greyScale = randFloat(0.5686f, 0.8039f);
		col = glm::vec4(greyScale, greyScale, greyScale, 0.9);
		origTrans = col.w;
	}
}

void Particle::update(float ltime, float timeIncr)
{
	if (stagger <= 0) {
		//check lifespan
		if (ltime > endLife) {
			rebirth(ltime);
		}

		//make particle smaller 
		scale -= scale / 350.0;

		//increase particle speed
		vel = 1.01f * vel;

		//move particle up
		pos += vel;

		float division = lifespan / timeIncr;
		//fade
		if (col.w > 0) {
			col.w -= origTrans / division;
		}

		//change color
		if (!smoke && col.y > 0.1686) {
			col.y -= col.y / division;
		}
		
	}
	else {
		if (--stagger == 0) {
			rebirth(ltime);
		}
	}
}

void Particle::drawFirePlace(glm::vec3 loc, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos)
{
	if (stagger <= 0) {
		//set handles for the particle
		glUniform4f(h_color, col.x, col.y, col.z, col.w);

		glm::vec3 curPos = glm::vec3(loc.x + pos.x, loc.y + pos.y, loc.z + pos.z);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), curPos);

		//billboard the particle
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	    glm::vec3 look = camPos - curPos;
	    look.y = 0.0;
	    look = glm::normalize(look);
	    glm::vec3 lookAt = glm::vec3(0.0, 0.0, -1.0);

	    float angle = glm::dot(lookAt, look);
	    glm::vec3 upAux = glm::cross(lookAt, look);
	    float ang = acos(angle)*180/3.14;
	    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang, upAux);

	    glm::mat4 result = trans * rot * glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
		glUniformMatrix4fv(h_ModelViewMat, 1, GL_FALSE, glm::value_ptr(result));

		// printf("pos: %f %f %f, life: %f, col: %f %f %f %f\n", loc.x + pos.x, loc.y + pos.y, loc.z + pos.z, lifespan, curCol.r, curCol.g, curCol.b, curCol.w);
		if (col.w > 0.05) {
			glDrawElements(GL_TRIANGLE_STRIP, indSize, GL_UNSIGNED_INT, 0);
		}
	}
}

void Particle::drawTorch(glm::vec3 loc, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos)
{
	if (stagger <= 0) {
		//set handles for the particle
		glUniform4f(h_color, col.x, col.y, col.z, col.w);

		glm::vec3 curPos = glm::vec3(loc.x + (pos.x / 1.85), loc.y + (pos.y / 1.25), loc.z + (pos.z / 1.85));
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), curPos);

		//billboard the particle
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	    glm::vec3 look = camPos - curPos;
	    look.y = 0.0;
	    look = glm::normalize(look);
	    glm::vec3 lookAt = glm::vec3(0.0, 0.0, -1.0);

	    float angle = glm::dot(lookAt, look);
	    glm::vec3 upAux = glm::cross(lookAt, look);
	    float ang = acos(angle)*180/3.14;
	    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), ang, upAux);

	    glm::mat4 result = trans * rot * glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
		glUniformMatrix4fv(h_ModelViewMat, 1, GL_FALSE, glm::value_ptr(result));

		// printf("pos: %f %f %f, life: %f, col: %f %f %f %f\n", loc.x + pos.x, loc.y + pos.y, loc.z + pos.z, lifespan, curCol.r, curCol.g, curCol.b, curCol.w);

		glDrawElements(GL_TRIANGLE_STRIP, indSize, GL_UNSIGNED_INT, 0);
	}
}