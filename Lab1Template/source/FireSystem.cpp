#include "FireSystem.h"
#include "shader.hpp"
#include "MatrixStack.h"

const glm::vec3 fireLoc[] = {glm::vec3(23.05, 0.4, -23.5), //center fireplace
							 glm::vec3(18.5, 2.9, -27.7), //torch locations
							 glm::vec3(27.6, 2.9, -27.7),
							 glm::vec3(18.5, 2.9, -19.3),
							 glm::vec3(27.6, 2.9, -19.3)
};

int FIRE_PARTICLE_1_ID = 8000;
int SMOKE_PARTICLE_1_ID = 8100;

FireSystem::FireSystem()
{

}

void FireSystem::loadBuffers()
{
    posBuf.push_back(-0.5f);
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	// 1
	posBuf.push_back(0.5f);
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(0.0f);
	// 2
	posBuf.push_back(-0.5f);
	posBuf.push_back(0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	// 3
	posBuf.push_back(0.5f);
	posBuf.push_back(0.5f);
	posBuf.push_back(0.0f);
	texBuf.push_back(1.0f);
	texBuf.push_back(1.0f);
	// indices
	indBuf.push_back(0);
	indBuf.push_back(1);
	indBuf.push_back(2);
	indBuf.push_back(3);

	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the texture coordinates array to the GPU
	glGenBuffers(1, &texBufID);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	glGenBuffers(1, &indBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
}

void FireSystem::init(TextureLoader *texLoader)
{
	// Initialize Shader
    pid = LoadShaders("Shaders/Fire_vert.glsl", "Shaders/Fire_frag.glsl");

    h_vertPos = GLSL::getAttribLocation(pid, "vertPosition");
    h_vertTexCoor = GLSL::getAttribLocation(pid, "vertTexCoords");
    h_tex = GLSL::getUniformLocation(pid, "tex");
    h_color = GLSL::getUniformLocation(pid, "color");
    h_ProjMat = GLSL::getUniformLocation(pid, "P");
    h_ModelMat = GLSL::getUniformLocation(pid, "M");
    h_ViewMat = GLSL::getUniformLocation(pid, "V");

    texLoader->LoadTexture((char *)"assets/smoke.bmp", FIRE_PARTICLE_1_ID);
    texLoader->LoadTexture((char *)"assets/smoke.bmp", SMOKE_PARTICLE_1_ID);

    loadBuffers();
    ltime = 0;
    timeIncr = 0.001;
    on = true;

	int numParticles = 1000;
	for (int iter = 0; iter < numParticles; iter++) {
		Particle temp;
		firePlace.push_back(temp);
		
		if (iter < numParticles / 10 * 9) {
			firePlace[iter].init(iter / 10 * 5, false);
		}
		else {
			firePlace[iter].init(iter - (numParticles / 10 * 9) / 10 * 10, true);
		}
	}

	// printf("initalizing resulted in error %d\n", glGetError());
    GLSL::checkVersion();
    assert(glGetError() == GL_NO_ERROR);
    std::cout << "Successfully initialized fire particle system\n";
}

void FireSystem::enableTex(int targetTex)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, targetTex);

	//mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLSL::enableVertexAttribArray(h_vertTexCoor);
	glBindBuffer(GL_ARRAY_BUFFER, texBufID);
	glVertexAttribPointer(h_vertTexCoor, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void FireSystem::draw(Camera *cam, glm::mat4 viewMat)
{
	if (!on) {
		return;
	}

	glUseProgram(pid);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set projection matrix
	MatrixStack proj;
	proj.pushMatrix();
	cam->applyProjectionMatrix(&proj);
	glUniformMatrix4fv(h_ProjMat, 1, GL_FALSE, glm::value_ptr( proj.topMatrix()));
	proj.pushMatrix();

	glUniformMatrix4fv(h_ViewMat, 1, GL_FALSE, glm::value_ptr(viewMat));

	//enables the buffers
	GLSL::enableVertexAttribArray(h_vertPos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

	enableTex(FIRE_PARTICLE_1_ID);
	glm::vec3 camPos = cam->getPosition();

	int curTorch = 1;
	int curTex = FIRE_PARTICLE_1_ID;

	ltime += timeIncr;
	for(int iter = 0; iter < firePlace.size(); iter++) {
		if (firePlace[iter].isSmokey() && curTex != SMOKE_PARTICLE_1_ID) {
			enableTex(SMOKE_PARTICLE_1_ID);
		}
		else if (!firePlace[iter].isSmokey() && curTex != FIRE_PARTICLE_1_ID) {
			enableTex(FIRE_PARTICLE_1_ID);
		}

		//center fireplace
		firePlace[iter].update(ltime, timeIncr);
		firePlace[iter].drawFirePlace(fireLoc[0], h_color, h_ModelMat, (int)indBuf.size(), camPos);

		//torches
		firePlace[iter].drawTorch(fireLoc[curTorch], h_color, h_ModelMat, (int)indBuf.size(), camPos);
		firePlace[iter].drawTorch(fireLoc[curTorch + 1], h_color, h_ModelMat, (int)indBuf.size(), camPos);
		curTorch = (curTorch ==  1) ? 3 : 1; //switch which torches are drawn on
	}

	//disable buffers
	GLSL::disableVertexAttribArray(h_vertTexCoor);
	GLSL::disableVertexAttribArray(h_vertPos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_TEXTURE_2D);
}

void FireSystem::toggle()
{
	on = !on;
}