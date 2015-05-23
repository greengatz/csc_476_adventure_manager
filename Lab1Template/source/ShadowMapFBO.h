/**
 * ShadowMapFBO.h - Header file for Terrain object.
 * @author Brandon Clark
 */

#pragma once
#ifndef _SHADOWMAPFBO_H_
#define _SHADOWMAPFBO_H_

#include "GLSL.h"
#include <vector>
#include "RenderingHelper.h"

class ShadowMapFBO
{
    public:
        ShadowMapFBO();

        ~ShadowMapFBO();

        bool init(unsigned int windowWidth, unsigned int windowHeight);

        void bindForWriting();

        void bindForReading(GLenum textureUnit);

    private:
        GLuint m_fbo;
        GLuint m_shadowMap;
};

#endif