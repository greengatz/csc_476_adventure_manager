#version 120
attribute vec4 vertPos; // in object space
attribute vec3 vertNor; // in object space
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying float fogDist;

varying vec2 vTexCoord;

void main()
{
	vec4 norm = (uViewMatrix * uModelMatrix) * vec4(vertNor, 0.0);
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertPos;
   
	vTexCoord = aTexCoord;

	//fog
	vec4 pos = uViewMatrix * uModelMatrix * vertPos;
	fogDist = abs(pos.z / pos.w);
}
