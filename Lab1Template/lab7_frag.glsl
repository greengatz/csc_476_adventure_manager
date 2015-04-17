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

//Switch toggle for coloring
uniform int terrainToggle;

varying vec3 color; // passed from the vertex shader
varying vec4 pos;
varying vec3 normal;

void main()
{
	vec3 n,e,l1,l2,h1,h2,color,light1,light2;
	float intensity = 0.8, result;
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


	if (terrainToggle == 1)
	{
		vec4 texColor1 = texture2D(uTexUnit, vTexCoord);
		gl_FragColor = texColor1 * 2.0 * vec4(color.r, color.g, color.b, 1.0);
	}
	else
	{
		gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
	}
}
