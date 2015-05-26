#version 120
//Texture
uniform sampler2D uTexUnit;

varying vec2 vTexCoord;
varying vec4 pos;
varying vec3 normal;

varying float fogDist;

void main()
{
	vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
	// float fogFactor = (5 - fogDist)/(5 - 8); //linear fog equation
	float fogFactor = 1.0 - 1.0 / exp(fogDist * 0.035); //fog density is 0.175
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4 targetCol = texture2D(uTexUnit, vTexCoord);
	gl_FragColor = mix(targetCol, fogColor, fogFactor);

	// gl_FragColor = texture2D(uTexUnit, vTexCoord);
}
