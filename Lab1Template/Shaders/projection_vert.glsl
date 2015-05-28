#version 120

attribute vec4 vertPos;
attribute vec3 vertNor;
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform mat4 uProjMatrixShadow;

uniform vec4 uLight;
uniform int leafToggle;

varying vec4 point;
varying vec2 vTexCoord;
varying float fogDist;

void shadowVertex()
{
	point = uProjMatrixShadow * vertPos;
}

void addFog()
{
	vec4 pos = uViewMatrix * uModelMatrix * vertPos;
	fogDist = abs(pos.z / pos.w);
}

void main()
{
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * uProjMatrixShadow * vertPos;

   gl_FrontColor = vec4(0.1, 0.1, 0.1, 1.0);

   shadowVertex();

   addFog();

   if (leafToggle == 1)
   {
   	vTexCoord = aTexCoord;
   }
}