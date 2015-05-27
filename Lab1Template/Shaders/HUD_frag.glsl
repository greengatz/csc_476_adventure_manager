#version 120
//Texture
uniform sampler2D uTexUnit;

varying vec2 vTexCoord;
varying vec4 pos;
varying vec3 normal;

void main()
{
	vec4 targetCol = texture2D(uTexUnit, vTexCoord);
	gl_FragColor = targetCol;
}