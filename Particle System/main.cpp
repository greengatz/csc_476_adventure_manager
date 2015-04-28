#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif
#include <iostream>
#include <vector>
#include <memory>
#include "GLSL.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "Particle.h"
#include "Program.h"

using namespace std;

bool keyToggles[256] = {false};
float t = 0.0f;
float h = 0.1f;
glm::vec3 g(0.0f, 0.0f, 0.0f);
vector<shared_ptr<Particle>> particles; // !C++11: vector<Particle*> particles;
Program prog;
Camera camera;

// OpenGL handle to texture data
Texture texture;
GLint h_texture0;

// Display time to control fps
float t0_disp = 0.0f;
float t_disp = 0.0f;

void loadScene()
{
	t = 0.0f;
	h = 0.001f;
	g = glm::vec3(0.0f, 0.05f, 0.0f);
	
	//One Fire ***
	int n = 10;
	for(int i = 0; i < n; ++i) {
		auto particle = make_shared<Particle>(); // !C++11: Particle *particle = new Particle();
		particles.push_back(particle);
		particle->load();
	}

	
	prog.setShaderNames("lab10_vert.glsl", "lab10_frag.glsl");
	texture.setFilename("alpha.bmp");
}

void initGL()
{
	//////////////////////////////////////////////////////
	// Initialize GL for the whole scene
	//////////////////////////////////////////////////////
	
	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//////////////////////////////////////////////////////
	// Intialize textures
	//////////////////////////////////////////////////////
	
	// Load texture image
	texture.setUnit(0);
	texture.setName("alphaTexture");
	texture.init();
	
	//////////////////////////////////////////////////////
	// Intialize the shaders
	//////////////////////////////////////////////////////
	
	prog.init();
	prog.addTexture(&texture);
	
	//////////////////////////////////////////////////////
	// Initialize the particles
	//////////////////////////////////////////////////////
	
	// !C++11
	//	for(vector<Particle*>::iterator p = particles.begin(); p != particles.end(); ++p) {
	//		(*p)->init(&prog);
	//	}
	for(auto particle : particles) {
		particle->init(&prog);
	}
	
	//////////////////////////////////////////////////////
	// Final check for errors
	//////////////////////////////////////////////////////
	GLSL::checkVersion();
}

void reshapeGL(int w, int h)
{
	// Set view size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	// Set camera aspect ratio
	camera.setAspect((float)w/h);
}

void drawGL()
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable backface culling
	if(keyToggles['c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles['l']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	// Create matrix stacks
	MatrixStack P, MV;
	// Apply camera transforms
	P.pushMatrix();
	camera.applyProjectionMatrix(&P);
	MV.pushMatrix();
	camera.applyViewMatrix(&MV);
	
	// Bind the program
	prog.bind();
	glUniformMatrix4fv(prog.getUniform("P"), 1, GL_FALSE, glm::value_ptr(P.topMatrix()));
	
	
	//One Fire ***
	for(auto particle : particles) {
		particle->draw(&MV);
	}
	
	// Unbind the program
	prog.unbind();

	// Pop stacks
	MV.popMatrix();
	P.popMatrix();

	// Double buffer
	glutSwapBuffers();
}

void mouseGL(int button, int state, int x, int y)
{
	int modifier = glutGetModifiers();
	bool shift = modifier & GLUT_ACTIVE_SHIFT;
	bool ctrl  = modifier & GLUT_ACTIVE_CTRL;
	bool alt   = modifier & GLUT_ACTIVE_ALT;
	camera.mouseClicked(x, y, shift, ctrl, alt);
}

void motionGL(int x, int y)
{
	camera.mouseMoved(x, y);
}

void keyboardGL(unsigned char key, int x, int y)
{
	keyToggles[key] = !keyToggles[key];
	switch(key) {
		case 27:
			// ESCAPE
			exit(0);
			break;
	}
}

// Sort particles by their z values in camera space
class ParticleSorter {
public:
	// !C++11 bool operator()(const Particle *p0, const Particle *p1) const
	bool operator()(const shared_ptr<Particle> p0, const shared_ptr<Particle> p1) const
	{
		// Particle positions in world space
		const glm::vec3 &x0 = p0->getPosition();
		const glm::vec3 &x1 = p1->getPosition();
		// Particle positions in camera space
		glm::vec4 x0w = C * glm::vec4(x0, 1.0f);
		glm::vec4 x1w = C * glm::vec4(x1, 1.0f);
		return x0w.z < x1w.z;
	}
	
	glm::mat4 C; // current camera matrix
};
ParticleSorter sorter;

void idleGL()
{
	if(keyToggles[' ']) {

		//One Fire ***
		for(auto particle : particles) {
			particle->update(t, h, g, keyToggles);
		}
		t += h;
	}
	
	// Display every 60 Hz
	t_disp = glutGet(GLUT_ELAPSED_TIME);
	if(t_disp - t0_disp > 1000.0f/60.0f) {
		// Sort the particles by Z
		MatrixStack temp;
		camera.applyViewMatrix(&temp);
		glm::mat4 V = temp.topMatrix();
		sorter.C = glm::transpose(glm::inverse(V)); // glm is transposed!
		sort(particles.begin(), particles.end(), sorter);
		// Ask for refresh
		glutPostRedisplay();
		t0_disp = t_disp;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Your Name");
	glutMouseFunc(mouseGL);
	glutMotionFunc(motionGL);
	glutKeyboardFunc(keyboardGL);
	glutReshapeFunc(reshapeGL);
	glutDisplayFunc(drawGL);
	glutIdleFunc(idleGL);
	loadScene();
	initGL();
	glutMainLoop();
	return 0;
}
