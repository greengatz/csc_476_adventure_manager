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
        void drawChar(GLint h_ModelMatrix, GLint h_vertPos, 
                GLint h_vertNor, GLint h_aTexCoord, GLint h_boneFlag,
                GLint h_boneIds, GLint h_boneWeights,
                GLint h_boneTransforms);

    private:
        const aiScene* scene;
        Assimp::Importer* importer;
        TextureLoader texLoader;

        aiNode* root;
        aiMesh** meshes;

        unsigned int numInd;
        unsigned int* indices;
        unsigned int boneCount;
        // keep track of all the bones
        float* normals;
        float* positions;
        float* texture;

        unsigned int* numBones; // number of bones affecting a given vertex
        unsigned int* boneId; // 5 per vertex
        float* boneWeight; // 5 per vertex
        

        GLuint posBuf;
        GLuint norBuf;
        GLuint indBuf;
        GLuint texBuf;
        GLuint boneIdBuf;
        GLuint boneWeightBuf;
        GLuint boneTransforms;

        vec3 position;

        void recursiveDraw(aiNode* toDraw);
        void recursivePrint(const aiNode* toPrint, int level, aiMesh** meshes);
};

#endif
