#version 120
attribute vec4 vertPos; // in object space
attribute vec3 vertNor; // in object space
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

//Switch toggle for coloring
uniform int terrainToggle;

// bone adjustments
const int MAX_BONES = 100;
uniform int boneToggle;
attribute vec4 boneIds;
attribute vec4 boneWeights;
uniform mat4 bones[MAX_BONES];

varying vec3 color; // Pass to fragment shader
varying vec4 pos;
varying vec3 normal;	
varying vec2 vTexCoord;

void main()
{
    vec4 norm = (uViewMatrix * uModelMatrix) * vec4(vertNor, 0.0);
    normal = norm.xyz;
    pos = uViewMatrix * uModelMatrix * vertPos;
    gl_Position = uProjMatrix * pos;
   
	if (terrainToggle == 1)
	{
		vTexCoord = aTexCoord;
	}
}
