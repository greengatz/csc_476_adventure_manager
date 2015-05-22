#version 120
attribute vec4 vertPos; // in object space
attribute vec3 vertNor; // in object space
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

//Switch toggle for coloring
uniform int terrainToggle;

//GUI toggle
uniform int flag;

// bone adjustments
const int MAX_BONES = 100;
uniform int boneToggle;
attribute ivec4 boneIds;
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
    //gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertPos;
    gl_Position = uProjMatrix * pos;
   
	if(flag == 1)
	{
		gl_Position = uModelMatrix * vertPos;
		vTexCoord = aTexCoord;
	}

	if (terrainToggle == 1)
	{
		vTexCoord = aTexCoord;
	}

    if (boneToggle == 1)
    {
        mat4 boneTrans = bones[boneIds[0]] * boneWeights[0];
        boneTrans += bones[boneIds[1]] * boneWeights[1];
        boneTrans += bones[boneIds[2]] * boneWeights[2];
        boneTrans += bones[boneIds[3]] * boneWeights[3];

        gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * boneTrans * vertPos;
        pos = uViewMatrix * boneTrans * uModelMatrix * vertPos;
        //normal = boneTrans * vec4(1.0, 1.0, 1.0, 0.0);
        normal = (boneTrans * vec4(normal, 1.0)).xyz;
    }
}
