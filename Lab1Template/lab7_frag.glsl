#version 120
//Texture
uniform sampler2D uTexUnit;
varying vec2 vTexCoord;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;
uniform float option;
uniform int flag;

//Switch toggle for coloring
uniform int terrainToggle;

varying vec3 color; // passed from the vertex shader
varying vec4 pos;
varying vec3 normal;

uniform int inTav;
varying float fogDist;

void main()
{
	vec3 n,e,l1,l2,h1,h2,color,light1,light2;
	float intensity = 1.0, result;
	n = normalize(normal);
	e = normalize((vec3(0.0,0.0,0.0) - pos.xyz));
	l1 = normalize(lightPos1 - pos.xyz);
	l2 = normalize(lightPos2 - pos.xyz);
	h1 = normalize(l1 + e);
	h2 = normalize(l2 + e);
	light1 = (ka + (kd * max(dot(l1,n), 0.0)) + (ks * pow(max(dot(h1,n), 0.0), s))) * intensity;
	intensity = 0.2;
	light2 = (ka + (kd * max(dot(l2,n), 0.0)) + (ks * pow(max(dot(h2,n), 0.0), s))) * intensity;
	color = light1 + light2;

	if (inTav == 0)
	{
		vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
		// float fogFactor = (5 - fogDist)/(5 - 8); //linear fog equation
		float fogFactor = 1.0 - 1.0 / exp(fogDist * 0.15); //fog density is 0.05
   		fogFactor = clamp(fogFactor, 0.0, 1.0);
	}

	if (terrainToggle == 1 || flag == 1)
	{
		vec4 texColor1 = texture2D(uTexUnit, vTexCoord);
		vec4 targetCol = texColor1 * 2.0 * vec4(color.r, color.g, color.b, 1.0);
		gl_FragColor = mix(targetCol, fogColor, fogFactor);

		// vec4 texColor1 = texture2D(uTexUnit, vTexCoord);
		// gl_FragColor = texColor1 * 2.0 * vec4(color.r, color.g, color.b, 1.0);
	}
	else
	{
		gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
	}
}
