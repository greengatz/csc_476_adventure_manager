#include "charDae.h"

CharDae::CharDae(const string source) {
    Assimp::Importer importer;
    scene = importer.ReadFile(source, 0);

    if(!scene) {
        cout << "couldn't read dae\n";
        return;
    }

    int i;
    vector<unsigned int> indeces;
    for(i = 0; i < scene->mRootNode->mNumMeshes; i++) {
        indeces.push_back(scene->mRootNode->mMeshes[i]);
    }

    glGenBuffers(1, &vao);
    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    // put the data in the buffer
    glBufferData(GL_ARRAY_BUFFER, indeces.size()*sizeof(float), &indeces[0], GL_STATIC_DRAW);
}

// go through hierarchy of nodes in scene
void CharDae::recursiveDraw(const aiNode* node) {
    // draw this node

//    glBindVertexArray();
    //
    GLSL::enableVertexAttribArray(vao);
    
    // seg faulting TODO
    //glDrawElements(GL_TRIANGLES, node->mNumMeshes, GL_UNSIGNED_INT, 0);
    int i;
    GLSL::disableVertexAttribArray(vao);

    // draw its children
}

void CharDae::drawChar() {
    if(scene) {
//        cout << "drawing a dae\n";
        recursiveDraw(scene->mRootNode);
    }
}
