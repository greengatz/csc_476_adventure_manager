#version 120
attribute vec4 vertPos; // in object space
attribute vec3 vertNor; // in object space
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

//Toggle for shading the leafs
uniform int leafToggle

varying vec4 pos;
varying vec3 normal; 
varying vec2 vTexCoord;

void main()
{
   vec4 norm = (uViewMatrix * uModelMatrix) * vec4(vertNor, 0.0);
   normal = norm.xyz;
   pos = uViewMatrix * uModelMatrix * vertPos;
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertPos;
   
   vTexCoord = aTexCoord;
}
