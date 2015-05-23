#version 120

attribute vec4 vertPos;
attribute vec3 vertNor;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main()
{
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertPos;
}