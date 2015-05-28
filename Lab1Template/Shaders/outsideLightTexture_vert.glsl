#version 120
attribute vec4 vertPos;
attribute vec3 vertNor;
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec4 pos;
varying vec3 normal;	
varying vec2 vTexCoord;
//fog
varying float fogDist;

void main()
{
    vec4 norm = (uViewMatrix * uModelMatrix) * vec4(vertNor, 0.0);
    normal = norm.xyz;
    pos = uViewMatrix * uModelMatrix * vertPos;
    gl_Position = uProjMatrix * pos;

	vTexCoord = aTexCoord;

	fogDist = abs(pos.z / pos.w);
}