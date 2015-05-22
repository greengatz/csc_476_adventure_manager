#include "charDae.h"

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
    meshInd = 1;

    cout << "root " << scene->mRootNode << "\n";
    root = scene->mRootNode;
    meshes = scene->mMeshes;
    cout << "scene " << scene << "\n";
    cout << "meshes " << meshes << "\n";
    cout << "facees in mesh 1 " << meshes[meshInd]->mNumFaces << "\n";
    cout << "number of bones " << meshes[meshInd]->mNumBones << "\n";

    position[0] = 26.56f; // magic to put in view
    position[1] = 0.0f;
    position[2] = -30.77f;

    numInd = meshes[meshInd]->mNumVertices;
    // TODO iterate through every mesh

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
    texture = (float*) malloc(numInd * 3 * sizeof(float));
    memcpy(positions, meshes[meshInd]->mTextureCoords, numInd * 3 * sizeof(float));

    glGenBuffers(1, &texBuf);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 3 * sizeof(float), texture, GL_STATIC_DRAW);


    // TODO bones
    // boneNum = max number of bones for any vertex
    // boneId -> boneNum ids for every vertex (which bones affect this vertex)
    // weights -> boneNum weights for every vertex (how much those bones affect this vertex)
    // bones -> a mat4 for every bone containing its current model transform
    //       -> this will update with time

    // aiMesh -> mBones and mNumBones
    // aiBone -> mWeights and mNumWeights
    // aiVertexWeight -> vertexId -> u int for which vertex
    //                -> mWeight  -> how much this bone affects that vertex (float, 0.0 - 1.0)
    
    // keep a count for every vertex
    // 220 of 7xxx indices are affected by 2 bones
    // will 5 bones be enough for every vertex? this number can change, but we want it tight

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

    // TODO make the model buffer
    // will need to update that buffers data pretty frequently...
    // GL_UNIFORM_BUFFER
    //glGenBuffers(1, &boneTransforms);
    floatModel = (float*) calloc(sizeof(float) * 16, boneCount);

    /*for(i = 0; i < numInd; i++) {
        cout << "vertex " << i << " has " << numBones[i] << " bones: ";
        for(j = 0; j < numBones[i]; j++) {
            cout << " bone " << boneWeight[i*5 + j];
        }
        cout << "\n";
    }*/

    cout << "finished loading " << source << "\n\n";
}

void CharDae::recursivePrint(const aiNode* node, int level, aiMesh** meshes) {
    int i, j;
    
    for (i = 0; i < level; i++) {
        printf("  -");
    }
    cout << "num meshes " << node->mNumMeshes << "\n";

    for (j = 0; j < node->mNumMeshes; j++) {
        for (i = 0; i < level; i++) {
            printf("  -");
        }
        cout << "mesh " << node->mMeshes[j];
        cout << ", face count " << meshes[node->mMeshes[j]]->mNumFaces; 
        cout << ", bones? " << meshes[node->mMeshes[j]]->HasBones(); 
        cout << ", normals? " << meshes[node->mMeshes[j]]->HasNormals(); 
        cout << ", positions? " << meshes[node->mMeshes[j]]->HasPositions(); 
        cout << "\n";
    }
    
    for (i = 0; i < level; i++) {
        printf("  -");
    }
    cout << "num children " << node->mNumChildren << "\n";

    for (i = 0; i < node->mNumChildren; i++) {
        recursivePrint(node->mChildren[i], level + 1, meshes);
    }
}

// go through hierarchy of nodes in scene
void CharDae::recursiveDraw(aiNode* node) {
    int i;
    // draw this node

//    glBindVertexArray();
    //
    //GLSL::enableVertexAttribArray(vao);
    //cout << "root " << node << "\n";
    //cout << "children " << node->mNumChildren << "\n";
    cout << "in\n";
    for (i = 0; i < node->mNumMeshes; i++) {
        // draw every one of our meshes
        cout << "i=" << i << "\n";
        cout << "mesh: " << node->mMeshes[i];
        cout << ", faces: " << meshes[node->mMeshes[i]]->mNumFaces; 
        cout << "\n";

        //TODO
        // bind positions, normals, indices
        // try to draw
        // glBindBuffer(GL_ARRAY_BUFFER, posBuf)
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf)
    }

    //glDrawElements(GL_TRIANGLES, node->mNumMeshes, GL_UNSIGNED_INT, 0);
    //GLSL::disableVertexAttribArray(vao);

    cout << "recursing...\n";
    // draw its children
    for (i = 0; i < node->mNumChildren; i++) {
        recursiveDraw(node->mChildren[i]);
    }
}


void CharDae::updateBones() {
    int bones = meshes[meshInd]->mNumBones;
    int i, j, k;

    //cout << "updating " << bones << " bones\n";

    boneModels.clear();
    for (i = 0; i < bones; i++) {
    //    cout << "bone " << i << "\n";
        aiMatrix4x4 boneModel = meshes[meshInd]->mBones[i]->mOffsetMatrix;
        boneModels.push_back(mat4(1.0));
        for(j = 0; j < 4; j++) {
            for(k = 0; k < 4; k++) {
      //          cout << boneModel[j][k] << " ";
                //floatModel[(i * 16) + (j * 4) + k] = boneModels[i][j][k];
                floatModel[(i * 16) + (j * 4) + k] = boneModels[i][j][k];
                //cout << floatModel[i * 16 + j * 4 + k] << " ";
            }
            //cout << "\n";
        }
        //cout << "\n";
    }
}


void CharDae::drawChar(GLint h_ModelMatrix, GLint h_vertPos, 
            GLint h_vertNor, GLint h_aTexCoord, GLint h_boneFlag, 
            GLint h_boneIds, GLint h_boneWeights, 
            GLint h_boneTransforms) {
    if(root == NULL) {
        return;
    }
    const aiScene* last = importer->GetScene();
 
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
    // gl buff, coords, id
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    int texNum = 5800;
    glBindTexture(GL_TEXTURE_2D, texNum); // what is this?

    GLSL::enableVertexAttribArray(h_aTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
    // model transform
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 result = translate * glm::scale(mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(result));

    // enable bones
    glUniform1i(h_boneFlag, 1);

    // bone id
    GLSL::enableVertexAttribArray(h_boneIds);
    glBindBuffer(GL_ARRAY_BUFFER, boneIdBuf);
    glVertexAttribPointer(h_boneIds, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    // bone weight
    GLSL::enableVertexAttribArray(h_boneWeights);
    glBindBuffer(GL_ARRAY_BUFFER, boneWeightBuf);
    glVertexAttribPointer(h_boneWeights, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // bone models
    updateBones();
    //GLSL::enable

    // bind buffer
    // buffer data
    // push buffer

    // TODO
    // scene -> animation -> node animation
    // bone transforms
    // we will pass several bones
    // each bone will have an ID and a Weight
    // these will be new buffers
    glUniformMatrix4fv(h_boneTransforms, boneCount, (GLboolean) false, 
            floatModel);

    // actual draw call
    glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);

    GLSL::disableVertexAttribArray(h_vertPos); // position
    GLSL::disableVertexAttribArray(h_vertNor); // normals
    GLSL::disableVertexAttribArray(h_aTexCoord); // texture
    GLSL::disableVertexAttribArray(h_boneIds); // texture
    GLSL::disableVertexAttribArray(h_boneWeights); // texture
    glUniform1i(h_boneFlag, 0);
}
