#version 120
//Texture
uniform sampler2D uTexUnit;

varying vec2 vTexCoord;
varying vec4 pos;
varying vec3 normal;

void main()
{
	vec4 targetCol = texture2D(uTexUnit, vTexCoord);
	if(targetCol.g == 1.0f)
	{
		discard;
	}
	gl_FragColor = targetCol;
}