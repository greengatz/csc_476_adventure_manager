#version 120

attribute vec4 vertPos;
attribute vec3 vertNor;
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec2 TexCoordOut;

void main()                                                                         
{                                                                                   
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertPos;
    TexCoordOut = aTexCoord;                                                  
}