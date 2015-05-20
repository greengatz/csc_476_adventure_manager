#include "charDae.h"

CharDae::CharDae(const string source) {
    int i, j;
    cout << "\n\ntrying to load " << source << "\n";

    scene = importer.ReadFile(source, aiProcess_GenNormals);

    if(!scene) {
        cout << "couldn't read dae\n";
        cout << "reason: " << importer.GetErrorString() << "\n";
        root = NULL;
        return;
    }
    cout << "root " << scene->mRootNode << "\n";
    root = scene->mRootNode;
    meshes = scene->mMeshes;
    cout << "scene " << scene << "\n";
    cout << "meshes " << meshes << "\n";
    cout << "facees in mesh 0 " << meshes[0]->mNumFaces << "\n";
    cout << "number of bones " << meshes[0]->mNumBones << "\n";

    position[0] = 26.56f; // magic to put in view
    position[1] = 0.0f;
    position[2] = -30.77f;

    numInd = meshes[0]->mNumVertices;
    // TODO iterate through every mesh

    // positions
    positions = (float*) malloc(numInd * 3 * sizeof(float));
    memcpy(positions, meshes[0]->mVertices, numInd * 3 * sizeof(float));

    glGenBuffers(1, &posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 3 * sizeof(float), positions, GL_STATIC_DRAW);

    // normals
    normals = (float*) malloc(meshes[0]->mNumVertices * 3 * sizeof(float));
    memcpy(normals, meshes[0]->mNormals, numInd * 3 * sizeof(float));

    glGenBuffers(1, &norBuf);
    glBindBuffer(GL_ARRAY_BUFFER, norBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 3 * sizeof(float), normals, GL_STATIC_DRAW);

    // indices (face)
    indices = (unsigned int*) malloc(meshes[0]->mNumFaces * 3 * sizeof(unsigned int));
    for (i = 0; i < meshes[0]->mNumFaces; i++) {
        const aiFace* face = &(meshes[0]->mFaces[i]);
        memcpy(&indices[i * 3], face->mIndices, 3 * sizeof(unsigned int));
    }

    glGenBuffers(1, &indBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[0]->mNumFaces * 3 * sizeof(int), indices, GL_STATIC_DRAW);


    //texture
    texture = (float*) malloc(numInd * 3 * sizeof(float));
    memcpy(positions, meshes[0]->mTextureCoords, numInd * 3 * sizeof(float));

    glGenBuffers(1, &texBuf);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glBufferData(GL_ARRAY_BUFFER, numInd * 3 * sizeof(float), texture, GL_STATIC_DRAW);
    
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

void CharDae::drawChar(GLint h_ModelMatrix, GLint h_vertPos, GLint h_vertNor, GLint h_aTexCoord) {
    if(root == NULL) {
        return;
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

    // texture
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

    // actual draw call
    glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);

    GLSL::disableVertexAttribArray(h_vertPos); // position
    GLSL::disableVertexAttribArray(h_vertNor); // normals
    GLSL::disableVertexAttribArray(h_aTexCoord); // texture
}
