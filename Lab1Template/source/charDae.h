#ifndef DAE
#define DAE

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLSL.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "TextureLoader.h"
#include <vector>

using namespace std;
using namespace glm;

class CharDae
{
    public:
        CharDae(const string file);
        void drawChar(GLint h_ModelMatrix, GLint h_vertPos, 
                GLint h_vertNor, GLint h_aTexCoord, GLint h_boneFlag,
                GLint h_boneIds, GLint h_boneWeights,
                GLint h_boneTransforms, float time);
        void startAnimation(string animation);
        int animChoice;

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
        unsigned int* boneId; // 4 per vertex
        float* boneWeight; // 4 per vertex
        //vector<Matrix4f>& boneModels;
        vector<mat4> boneModels;
        float* floatModel;
        
        float animStart;
       
        int meshInd; 

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
        void recursiveUpdate(const aiNode* toUpdate, float time);

        // methods to interpolate transform between animation frames
        aiQuaternion intRot(float time, const aiNodeAnim* nodeAnim);
        aiVector3D intScale(float time, const aiNodeAnim* nodeAnim);
        aiVector3D intTrans(float time, const aiNodeAnim* nodeAnim);
        void updateBones(float time);
        const aiNodeAnim* findNodeAnim(const aiAnimation* anim, const aiNode* toFind);


        int findBone(const aiNode* toFind);
};

#endif
