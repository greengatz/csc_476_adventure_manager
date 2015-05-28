#version 120
//Texture
uniform sampler2D uTexUnit;
varying vec2 vTexCoord;

uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 pos;
varying vec3 normal;
varying float fogDist;

void main()
{
	vec3 n,e,l1,h1,color,light;
	float intensity = 1.0, result;
	n = normalize(normal);
	e = normalize((vec3(0.0,0.0,0.0) - pos.xyz));
	l1 = normalize(lightPos - pos.xyz);
	h1 = normalize(l1 + e);
	light = (ka + (kd * max(dot(l1,n), 0.0)) + (ks * pow(max(dot(h1,n), 0.0), s))) * intensity;
	intensity = 0.2;

   vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
   // float fogFactor = (5 - fogDist)/(5 - 8); //linear fog equation
   float fogFactor = 1.0 - 1.0 / exp(fogDist * 0.15); //fog density is 0.05
   
   fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4 texColor1 = texture2D(uTexUnit, vTexCoord);
	vec4 targetCol = texColor1 * 2.0 * vec4(light.x, light.y, light.z, 1.0);
	gl_FragColor = mix(targetCol, fogColor, fogFactor);
}