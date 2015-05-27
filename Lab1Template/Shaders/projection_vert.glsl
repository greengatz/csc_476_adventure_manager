#version 120

attribute vec4 vertPos;
attribute vec3 vertNor;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform mat4 uProjMatrixShadow;

void main()
{
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * uProjMatrixShadow * vertPos;
   //vec4 transMat = uProjMatrix * uViewMatrix * uModelMatrix * uProjMatrixShadow * vertPos;
   //gl_Position = transMat * 1.0/transMat.w;
}