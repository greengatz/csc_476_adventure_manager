/*
    ShadowMapTechnique.cpp - Sets up shaders for shadow mapping.
*/

//#include "ogldev_util.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLSL.h"
#include "ShadowMapTechnique.h"
#include "shader.hpp"

ShadowMapTechnique::ShadowMapTechnique()
{
    pid = 0;
    h_ProjMatrix = 0;
    h_ViewMatrix = 0;
    h_ModelMatrix = 0;
    m_textureLocation = 0;
    //m_WVPLocation = 0;
}

bool ShadowMapTechnique::init()
{
    // Initialize Shader
    pid = LoadShaders( "Shaders/shadowmap_vert.glsl", "Shaders/shadowmap_frag.glsl");

    h_ProjMatrix = GLSL::getUniformLocation(pid, "uProjMatrix");
    h_ViewMatrix = GLSL::getUniformLocation(pid, "uViewMatrix");
    h_ModelMatrix = GLSL::getUniformLocation(pid, "uModelMatrix");

    //m_WVPLocation = GLSL::getUniformLocation(pid, "gWVP");
    m_textureLocation = GLSL::getUniformLocation(pid, "gShadowMap");

    GLSL::checkVersion();
    assert(glGetError() == GL_NO_ERROR);
}

/*void ShadowMapTechnique::setWVP(const mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}*/

void ShadowMapTechnique::enableProgram()
{
    glUseProgram(pid);
}

void ShadowMapTechnique::setTextureUnit(unsigned int textureUnit)
{
    glUniform1i(m_textureLocation, textureUnit);
}
