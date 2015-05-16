#include "Particle.h"

float Particle::randFloat(float low, float high)
{
	float num = rand() / (float)RAND_MAX;
	return (1.0f - num) * low + num * high;
}

Particle::Particle()
{
	mass = 0.01;
	damp = 0;
	lifespan = 1.0;
	scale = 1.0;
}

void Particle::init()
{
	endCol = glm::vec4(0.45098, 0.5098, 0.46274, 0.5);
	rebirth(0.0);
	mass = 0.01;
}

void Particle::rebirth(float curTime)
{
	damp = randFloat(0.0f, 0.01f);
	pos.x = randFloat(-0.25f, 0.2f);
	pos.y = randFloat(-0.15f, 0.15f);
	pos.z = randFloat(-0.25f, 0.2f);
	vel.x = randFloat(0.0f, 0.00001f);
	vel.y = randFloat(0.0f, 0.005f);
	vel.z = randFloat(0.0f, 0.00001f);
	lifespan = randFloat(0.08f, 0.125f);
	endLife = curTime + lifespan;
	// scale = randFloat(0.1f, 0.125f);
	scale = randFloat(0.05f, 0.075f);
	// scale = randFloat(0.01f, 0.04f);
	startCol = glm::vec4(1.0f, randFloat(0.15f, 0.35f), 0, randFloat(0.8f, 1.0f));
	curCol = startCol;
}

void Particle::update(float ltime, float timeIncr)
{
	// printf("time: %f %f\n", ltime, endLife);
	//check lifespan
	if (ltime > endLife) {
		rebirth(ltime);
	}

	//make particle smaller //min size is .025?
	float r = sqrt(pos.x * pos.x + pos.y + pos.y + pos.z * pos.z) / 2.0;
	// if (scale - (r / 100000) > 0) {
	// 	scale -= r / 100000;
	// }

	glm::vec3 f = glm::vec3(0, mass * 0.05, 0);
	//increase particle speed
	// vel += (timeIncr / mass) * f;
	vel = 1.01f * vel;

	//move particle up
	// pos += vel * timeIncr;
	pos += vel;

	float division = lifespan / timeIncr;

	//update color
	// curCol = (startCol - endCol) / (endLife - ltime) * .01; //.01 is the current delta time?
	// curCol = glm::vec4(curCol.x, curCol.y, curCol.z, curCol.w);
	if (curCol.y < 0.917f) {
		curCol.y += curCol.y / division;
	}

	if (curCol.z < 0.843f) {
		curCol.z += curCol.z / division;
	}

	if (curCol.w > 0) {
		curCol.w -= curCol.w / division * 0.75;
	}
	// printf("col updated to %f %f %f %f\n", curCol.x, curCol.y, curCol.z, curCol.w);
}

// void Particle::update(float t, float h, const glm::vec3 &g, const bool *keyToggles)
// {
// 	int C = -1; //not used ever
// 	glm::vec3 f = m * g; //f is ... what? g is always initialized as (0, .05, 0) not sure what that is...
// 	f = f + (-d * v); 
// 	float r = sqrt(x.x * x.x + x.y * x.y + x.z * x.z) / 2;
	
// 	if( t > tEnd) {
// 		rebirth(t);
// 	}

// 	if(keyToggles['v']){ //dont care about this
// 		m -= 0.01;
// 	}
// 	//makes the particle smaller
// 	if(scale - (r / 100000) > 0){
// 		scale -=  r / 100000;
// 	}
// 	//increases the g values of the particle color
// 	if(r*3 <= 1){
// 		color.y = 1 - r*3;
// 	}
// 	//velocity is increased?
// 	v += (h / m) * f; //h is time increment, .001
// 	x += h*v;
// 	color.w = (tEnd-t)/lifespan; //make more transparent
// 	if(keyToggles['f']){ //dont care about this... i think?
// 		if(x.y <= -3.0f){
// 			v.x = 0.0f - v.x;
// 			v.y = 0.0f - v.y;
// 			v.z = 0.0f - v.z;
// 		}
// 	}

// }

//need to clean up these parameters eventually
void Particle::draw(glm::vec3 loc, GLint h_scale, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos)
{
	//set handles for the particle
	glUniform1f(h_scale, scale);
	glUniform4f(h_color, curCol.r, curCol.g, curCol.b, curCol.w);

	glm::vec3 curPos = glm::vec3(loc.x + pos.x, loc.y + pos.y, loc.z + pos.z);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), curPos);

	//billboard the particle
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 look = camPos - curPos; //(treePosition + position + glm::vec3(0.0, 0.88, 0.0));
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

//need to clean up these parameters eventually
void Particle::drawFirePlace(glm::vec3 loc, GLint h_scale, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos)
{
	//set handles for the particle
	glUniform1f(h_scale, scale);
	glUniform4f(h_color, curCol.r, curCol.g, curCol.b, curCol.w);

	glm::vec3 curPos = glm::vec3(loc.x + pos.x, loc.y + pos.y, loc.z + pos.z);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), curPos);

	//billboard the particle
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 look = camPos - curPos; //(treePosition + position + glm::vec3(0.0, 0.88, 0.0));
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

//need to clean up these parameters eventually
void Particle::drawTorch(glm::vec3 loc, GLint h_scale, GLint h_color, GLint h_ModelViewMat, int indSize, glm::vec3 camPos)
{
	//set handles for the particle
	glUniform1f(h_scale, scale);
	glUniform4f(h_color, curCol.r, curCol.g, curCol.b, curCol.w);

	glm::vec3 curPos = glm::vec3(loc.x + (pos.x / 1.85), loc.y + (pos.y / 1.25), loc.z + (pos.z / 1.85));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), curPos);

	//billboard the particle
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 look = camPos - curPos; //(treePosition + position + glm::vec3(0.0, 0.88, 0.0));
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