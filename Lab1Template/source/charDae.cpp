#include "charDae.h"

CharDae::CharDae(const string source) {
    int i, j;
    cout << "\n\ntrying to load " << source << "\n";
    Assimp::Importer importer;

//    const aiScene* thing = importer.ReadFile(source, aiProcess_GenNormals);
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

    position[0] = 26.56f; // magic to put in view
    position[1] = 2.0f;
    position[2] = -30.77f;

    numInd = meshes[0]->mNumVertices;
    // TODO iterate through every mesh

    // generate index array
    unsigned int* faceArray = (unsigned int*) malloc(sizeof(unsigned int) * 3 * meshes[0]->mNumFaces);

    for (i = 0; i < meshes[0]->mNumFaces; i++) {
        const aiFace* face = &(meshes[0]->mFaces[i]);
        memcpy(&faceArray[i * 3], face->mIndices, 3*sizeof(unsigned int));
    }
    
    // positions
    glGenBuffers(1, &posBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[0]->mNumVertices*3*sizeof(float), meshes[0]->mVertices, GL_STATIC_DRAW);

    // normals
    glGenBuffers(1, &norBuf);
    glBindBuffer(GL_ARRAY_BUFFER, norBuf);
    cout << "num vertices " << meshes[0]->mNumVertices << "\n";
    glBufferData(GL_ARRAY_BUFFER, meshes[0]->mNumVertices*3*sizeof(float), meshes[0]->mNormals, GL_STATIC_DRAW);

    // indices (face)
    glGenBuffers(1, &indBuf);
    glBindBuffer(GL_ARRAY_BUFFER, indBuf);
    glBufferData(GL_ARRAY_BUFFER, meshes[0]->mNumFaces*3*sizeof(int), faceArray, GL_STATIC_DRAW);

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

    printf("beginning dae draw\n");
    cout << scene->mMeshes << "\n";
    cout << scene->mMeshes[0]->mNumVertices << "\n";
    
    printf("enabling buffers...\n");
    GLSL::enableVertexAttribArray(h_vertPos); // position
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer(h_vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLSL::enableVertexAttribArray(h_vertNor); // normals
    glBindBuffer(GL_ARRAY_BUFFER, norBuf);
    glVertexAttribPointer(h_vertNor, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf); // indices

    printf("beginning recursive draw\n");
  //  cout << "root" << root << "\n";
//    recursiveDraw(root);

    //
    // draw mesh 0
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(h_ModelMatrix, 1, GL_FALSE, glm::value_ptr(position));

//    cout << "here?" << scene << "\n";
    //aiMesh* nIndices = scene->mMeshes[0];
//    cout << "num vertices " << numInd << "\n";
//    cout << "out \n";
    glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);


//    printf("disabling buffers...\n");
    GLSL::disableVertexAttribArray(h_vertPos); // position
    GLSL::disableVertexAttribArray(h_vertNor); // normals
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    cout << meshes << "\n";

  
    printf("done drawing\n");
}
