#include "charDae.h"

enum daeTypes {
    samurai
};

enum animations {
    walk,
    punch,
    idle,
    animCount
};

int startAnim[][animCount] = {{1, 31, 71}};
int endAnim[][animCount] = {{30, 70, 100}};

int framesPerSec = 24;

CharDae::CharDae(const string source) {
    int i, j, k;
    cout << "\n\ntrying to load " << source << "\n";
    importer = new Assimp::Importer();
    scene = importer->ReadFile(source, aiProcess_GenNormals);

    if(!scene) {
        cout << "couldn't read dae\n";
        cout << "reason: " << importer->GetErrorString() << "\n";
        root = NULL;
        return;
    }

    // TODO remove this magic
    meshInd = 0;
    daeType = 0;
    animChoice = -1;

    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotate = 45.0f;

    //cout << "root " << scene->mRootNode << "\n";
    root = scene->mRootNode;
    meshes = scene->mMeshes;
    cout << "scene " << scene << "\n";
   /* cout << "meshes " << meshes << "\n";
    cout << "facees in mesh 1 " << meshes[meshInd]->mNumFaces << "\n";
    cout << "number of bones " << meshes[meshInd]->mNumBones << "\n";
    cout << "number of animations " << scene->mNumAnimations << "\n";
    cout << "duration of 1 " << scene->mAnimations[0]->mDuration << "\n";
    cout << "tps " << scene->mAnimations[0]->mTicksPerSecond << "\n";
    cout << "number of frames in 1 " << scene->mAnimations[0]->mChannels[0]->mNumRotationKeys << "\n";
*/

    //cout << "has tex? " << meshes[meshInd]->HasTextureCoords(0) << "\n";
    //cout << "num comps: " << meshes[meshInd]->mNumUVComponents[0] << "\n";
    position[0] = 26.56f; // magic to put in view
    position[1] = 0.0f;
    position[2] = -30.77f;

    numInd = meshes[meshInd]->mNumVertices;
    // recursivePrint(scene->mRootNode, 0, meshes);

    // positions
    positions = (float*) malloc(numInd * 3 * sizeof(float));
    memcpy(positions, meshes[meshInd]->mVertices, numInd * 3 * sizeof(float));

    glGenBuffers(1, &posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 3 * sizeof(float), positions, GL_STATIC_DRAW);


    // normals
    normals = (float*) malloc(meshes[meshInd]->mNumVertices * 3 * sizeof(float));
    memcpy(normals, meshes[meshInd]->mNormals, numInd * 3 * sizeof(float));

    glGenBuffers(1, &norBuf);
    glBindBuffer(GL_ARRAY_BUFFER, norBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 3 * sizeof(float), normals, GL_STATIC_DRAW);


    // indices (face)
    indices = (unsigned int*) malloc(meshes[meshInd]->mNumFaces * 3 * sizeof(unsigned int));
    for (i = 0; i < meshes[meshInd]->mNumFaces; i++) {
        const aiFace* face = &(meshes[meshInd]->mFaces[i]);
        memcpy(&indices[i * 3], face->mIndices, 3 * sizeof(unsigned int));
    }

    glGenBuffers(1, &indBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[meshInd]->mNumFaces * 3 * sizeof(int), indices, GL_STATIC_DRAW);


    // texture
    texture = (float*) malloc(numInd * 2 * sizeof(float));
    memcpy(positions, meshes[meshInd]->mTextureCoords, numInd * 2 * sizeof(float));

    glGenBuffers(1, &texBuf);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 2 * sizeof(float), texture, GL_STATIC_DRAW);


    // numBones, boneId, boneWeight
    // boneCount
    numBones = (unsigned int*) calloc(sizeof(unsigned int), numInd);
    boneId = (unsigned int*) calloc(sizeof(unsigned int) * 4, numInd);
    boneWeight = (float*) calloc(sizeof(float) * 4, numInd);
    aiVertexWeight boneVertex;

    // for every bone
    for(i = 0; i < meshes[meshInd]->mNumBones; i++) {
        // mark which vertices this bone affects
        for(j = 0; j < meshes[meshInd]->mBones[i]->mNumWeights; j++) {
            boneVertex = meshes[meshInd]->mBones[i]->mWeights[j];

            boneId[(boneVertex.mVertexId * 4) + numBones[boneVertex.mVertexId]] = i;
            boneWeight[(boneVertex.mVertexId * 4) + numBones[boneVertex.mVertexId]] = boneVertex.mWeight;

            numBones[boneVertex.mVertexId]++;
        }
    }

    // fill the bone Id buffer
    glGenBuffers(1, &boneIdBuf);
    glBindBuffer(GL_ARRAY_BUFFER, boneIdBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 4 * sizeof(unsigned int), boneId, GL_STATIC_DRAW);
    
    // fill the bone Weights buffer
    glGenBuffers(1, &boneWeightBuf);
    glBindBuffer(GL_ARRAY_BUFFER, boneWeightBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 4 * sizeof(float), boneWeight, GL_STATIC_DRAW);

    // make the model buffer
    // will need to update that buffers data pretty frequently...
    boneCount = meshes[meshInd]->mNumBones;
    floatModel = (float*) calloc(sizeof(float) * 16, boneCount);
    cout << "floatModel at " << floatModel << "\n";
    cout << "boneCount " << boneCount << "\n";

    cout << "finished loading " << source << "\n\n";
}

void CharDae::recursivePrint(const aiNode* node, int level, aiMesh** meshes) {
    int i, j;
    
    for (i = 0; i < level; i++) {
        printf("  -");
    }

    //cout << "mesh " << node->mMeshes[0];
    cout << "name " << node->mName.data;
    //cout << ", face count " << meshes[node->mMeshes[0]]->mNumFaces; 
    //cout << ", bones? " << meshes[node->mMeshes[0]]->HasBones(); 
    //cout << ", normals? " << meshes[node->mMeshes[0]]->HasNormals(); 
    //cout << ", positions? " << meshes[node->mMeshes[0]]->HasPositions(); 
    cout << "\n";
    
    for (i = 0; i < node->mNumChildren; i++) {
        recursivePrint(node->mChildren[i], level + 1, meshes);
    }
}

void CharDae::updateBones(float time) {
    recursiveUpdate(scene->mRootNode, time);
}

void CharDae::recursiveUpdate(const aiNode* toUpdate, float time) {
    aiMatrix4x4t<float> parent, us; // default to identity
    int i, j;
    int updateId = findBone(toUpdate);
    const aiAnimation* anim = scene->mAnimations[0];
    const aiNodeAnim* nodeAnim = findNodeAnim(anim, toUpdate);
    float timeOffset = 0;

    if(isAnimating()) {
        timeOffset = ((float) startAnim[daeType][animChoice]) / ((float) framesPerSec);
    } else if (lastAnim != -1) {
        // based on last animations end frame
        timeOffset = ((float) endAnim[daeType][lastAnim]) / ((float) framesPerSec);
    }

    us = toUpdate->mTransformation;
    if(nodeAnim != NULL) {
        // interpolate from where we are in the animation
        aiMatrix4x4t<float> sMat, pMat, rMat;
        aiVector3D scale = intScale(time - animStart + timeOffset, nodeAnim);
        aiVector3D pos = intTrans(time - animStart + timeOffset, nodeAnim);
        aiQuaternion rot = intRot(time - animStart + timeOffset, nodeAnim);

        sMat = aiMatrix4x4t<float>::Scaling(scale, sMat);
        pMat = aiMatrix4x4t<float>::Translation(pos, pMat);
        rMat = aiMatrix4x4t<float>(rot.GetMatrix());

        us = pMat * rMat * sMat;
    }

    // TODO should store this data, wasted lookups
    const aiNode* temp = toUpdate->mParent;
    while(temp != NULL) {
        const aiNodeAnim* pAnim = findNodeAnim(anim, temp);
        if(pAnim != NULL) {
            // use int parent
            aiMatrix4x4t<float> sMatP, pMatP, rMatP;
            aiVector3D scaleP = intScale(time - animStart + timeOffset, pAnim);
            aiVector3D posP = intTrans(time - animStart + timeOffset, pAnim);
            aiQuaternion rotP = intRot(time - animStart + timeOffset, pAnim);

            sMatP = aiMatrix4x4t<float>::Scaling(scaleP, sMatP);
            pMatP = aiMatrix4x4t<float>::Translation(posP, pMatP);
            rMatP = aiMatrix4x4t<float>(rotP.GetMatrix());

            parent = pMatP * rMatP * sMatP * parent;
        } else {
            parent = temp->mTransformation * parent;
        }
        temp = temp->mParent;
    }

    us = parent * us;
    

    if(updateId != -1) {
        us = us * meshes[meshInd]->mBones[updateId]->mOffsetMatrix;
        us = us.Transpose();

        for(i = 0; i < 4; i++) {
            for(j = 0; j < 4; j++) {
                floatModel[(updateId * 16) + (i * 4) + j] = us[i][j];
            } 
        }
    }

    for(i = 0; i < toUpdate->mNumChildren; i++) {
        recursiveUpdate(toUpdate->mChildren[i], time);
    }
}

int CharDae::findBone(const aiNode* toFind) {
    int i;
    for(i = 0; i < meshes[meshInd]->mNumBones; i++) {
        if(toFind->mName == meshes[meshInd]->mBones[i]->mName) {
            return i;
        }
    }
    return -1;
}

const aiNodeAnim* CharDae::findNodeAnim(const aiAnimation* anim, const aiNode* toFind) {
    int i;
    for(i = 0; i < anim->mNumChannels; i++) {
        if(anim->mChannels[i]->mNodeName == toFind->mName) {
            return anim->mChannels[i];
        }
    }
    return NULL;
}



// TODO, actually interpolate
aiQuaternion CharDae::intRot(float time, const aiNodeAnim* nodeAnim) {
    int i = 0; // int between i and i+1
    int key;
    
    for(i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) {
        if(time < (float) nodeAnim->mRotationKeys[i + 1].mTime) {
            key = i;
            break;
        }
    }

    const aiQuaternion startRot = nodeAnim->mRotationKeys[key].mValue;
    const aiQuaternion endRot = nodeAnim->mRotationKeys[key + 1].mValue;
    float dt = nodeAnim->mRotationKeys[key + 1].mTime - 
                nodeAnim->mRotationKeys[key].mTime;
    float factor = (time - nodeAnim->mRotationKeys[key].mTime) / dt;

    aiQuaternion ret;
    aiQuaternion::Interpolate(ret, startRot, endRot, factor);

    return ret;
}

// note: our animations don't change scale yet
aiVector3D CharDae::intScale(float time, const aiNodeAnim* nodeAnim) {
    return aiVector3D(1.0, 1.0, 1.0);
}

aiVector3D CharDae::intTrans(float time, const aiNodeAnim* nodeAnim) {
    int i = 0; // int between i and i+1
    int key;
    
    for(i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) {
        if(time < (float) nodeAnim->mPositionKeys[i + 1].mTime) {
            key = i;
            break;
        }
    }

    const aiVector3D startPos = nodeAnim->mPositionKeys[key].mValue;
    const aiVector3D endPos = nodeAnim->mPositionKeys[key + 1].mValue;
    float dt = nodeAnim->mPositionKeys[key + 1].mTime - 
                nodeAnim->mPositionKeys[key].mTime;
    float factor = (time - nodeAnim->mPositionKeys[key].mTime) / dt;

    aiVector3D ret;
    ret += startPos * (1.0f - factor); // TODO test this out
    ret += endPos * (factor);
   // aiVector3D::Interpolate(ret, startPos, endPos, factor);

    return ret;
}


void CharDae::startAnimation(string animation) {
    animStart = lastTime;

    animChoice = -1;
    if(strcmp(animation.c_str(), "run") == 0) {
        animChoice = walk;
    } else if(strcmp(animation.c_str(), "punch") == 0) {
        animChoice = punch;
    } else if(strcmp(animation.c_str(), "idle") == 0) {
        animChoice = idle;
    }

    if(animChoice != -1) {
        int numFrames = endAnim[daeType][animChoice] - startAnim[daeType][animChoice];
        // frames / fps
        endTime = (((float) numFrames) / ((float) framesPerSec)) + animStart;
    }
    lastAnim = animChoice;
}

bool CharDae::isAnimating() {
    return animChoice != -1;
}


void CharDae::drawChar(GLint h_ModelMatrix, GLint h_vertPos, 
            GLint h_vertNor, GLint h_aTexCoord, GLint h_boneFlag, 
            GLint h_boneIds, GLint h_boneWeights, 
            GLint h_boneTransforms, float time) {
    if(root == NULL) {
        return;
    }

    // prepare to start an animation
    lastTime = time;
    if (time > endTime) {
        animChoice = -1;
    }
    if (!isAnimating()) {
        animStart = lastTime;
    }

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
    
    // position
    GLSL::enableVertexAttribArray(h_vertPos);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // normals
    GLSL::enableVertexAttribArray(h_vertNor);
    glBindBuffer(GL_ARRAY_BUFFER, norBuf);
    glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // texture TODO
    int texNum = 7500;
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texNum); // what is this?

    GLSL::enableVertexAttribArray(h_aTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
   
    // model transform
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 result = translate * glm::rotate(glm::mat4(1.0f), rotate, glm::vec3(0, 1, 0)) * glm::scale(mat4(1.0f), glm::vec3(8.0f, 8.0f, 8.0f)) * glm::scale(mat4(1.0f), scale);
    glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(result));

    // enable bones
    glUniform1i(h_boneFlag, 1);

    // bone id
    GLSL::enableVertexAttribArray(h_boneIds);
    glBindBuffer(GL_ARRAY_BUFFER, boneIdBuf);
    glVertexAttribPointer(h_boneIds, 4, GL_INT, GL_FALSE, 0, 0);
    
    // bone weight
    GLSL::enableVertexAttribArray(h_boneWeights);
    glBindBuffer(GL_ARRAY_BUFFER, boneWeightBuf);
    glVertexAttribPointer(h_boneWeights, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // bone models
    updateBones(time);

    glUniformMatrix4fv(h_boneTransforms, boneCount, (GLboolean) false, floatModel);

    // actual draw call
    glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);

    GLSL::disableVertexAttribArray(h_vertPos); // position
    GLSL::disableVertexAttribArray(h_vertNor); // normals
    GLSL::disableVertexAttribArray(h_aTexCoord); // texture
    GLSL::disableVertexAttribArray(h_boneIds); // bone ids
    GLSL::disableVertexAttribArray(h_boneWeights); // bone weights
    
    glUniform1i(h_boneFlag, 0);
    glDisable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0); // what is this?
}
