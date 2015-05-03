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
attribute vec3 hudColor;
varying vec3 hudCol;

varying vec3 color; // Pass to fragment shader
varying vec4 pos;
varying vec3 normal;	
varying vec2 vTexCoord;

void main()
{
	vec4 norm = (uViewMatrix * uModelMatrix) * vec4(vertNor, 0.0);
	normal = norm.xyz;
	pos = uViewMatrix * uModelMatrix * vertPos;
	if(flag == 1)
	{
		gl_Position = uModelMatrix * vertPos;
		hudCol = hudColor;
		vTexCoord = aTexCoord;
	}
	else
	{
		gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vertPos;
		hudCol = vec3(1.0f, 0, 0);
	}

	if (terrainToggle == 1)
	{
		vTexCoord = aTexCoord;
	}
	//color = vec3(0.5, 0.5, 0.5);
}
