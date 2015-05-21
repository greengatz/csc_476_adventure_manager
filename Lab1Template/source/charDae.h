#ifndef DAE
#define DAE

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <stdio.h>
#include "GLSL.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "TextureLoader.h"

using namespace std;
using namespace glm;

class CharDae
{
    public:
        CharDae(const string file);
        void drawChar(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord);

    private:
        const aiScene* scene;
        Assimp::Importer importer;
        TextureLoader texLoader;

        aiNode* root;
        aiMesh** meshes;

        unsigned int numInd;
        unsigned int* indices;
        float* normals;
        float* positions;
        float* texture;

        GLuint posBuf;
        GLuint norBuf;
        GLuint indBuf;
        GLuint texBuf;

        vec3 position;

        void recursiveDraw(aiNode* toDraw);
        void recursivePrint(const aiNode* toPrint, int level, aiMesh** meshes);
};

#endif
