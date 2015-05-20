/*
    ShadowMapTechnique.h - Helps with setting attributes for shadow mapping
    @author Brandon Clark
 */

#ifndef SHADOW_MAP_TECHNIQUE_H
#define	SHADOW_MAP_TECHNIQUE_H

//#include "technique.h"
//#include "ogldev_math_3d.h"
#include "GLSL.h"
#include <vector>
#include "RenderingHelper.h"

class ShadowMapTechnique { // : public Technique {

public:

    ShadowMapTechnique();

    virtual bool init();

    //void setWVP(const mat4& WVP);
    void setTextureUnit(unsigned int textureUnit);
    void enableProgram();
private:

    //GLuint m_WVPLocation;

    GLuint pid;
    GLuint h_ProjMatrix;
    GLuint h_ViewMatrix;
    GLuint h_ModelMatrix;

    GLuint m_textureLocation;
};


#endif	/* SHADOW_MAP_TECHNIQUE_H */