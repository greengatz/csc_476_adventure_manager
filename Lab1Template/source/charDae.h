#ifndef DAE
#define DAE

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <stdio.h>
#include "GLSL.h"
#include <iostream>

using namespace std;

class CharDae
{
    public:
        CharDae(const string file);
        void drawChar();

    private:
        const aiScene* scene;
        GLuint vao;
        void recursiveDraw(const aiNode* toDraw);
};

#endif
