#version 120
//Texture
uniform sampler2D uTexUnit;
uniform sampler2D uTexNorUnit;
varying vec2 vTexCoord;
varying vec2 vNorCoord;

uniform vec3 lightPos1;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

//Switch toggle for coloring
uniform int terrainToggle;
uniform int normalToggle;

varying vec3 color; // passed from the vertex shader
varying vec4 pos;
varying vec3 normal;

void main()
{
	vec3 n,e,l1,h1,color,light1;
	float intensity = 1.0, result;
   n = normalize(normal);
   if (normalToggle == 1)
   {
      vec4 texColor2 = texture2D(uTexNorUnit, vTexCoord);
      vec3 norNor = vec3(texColor2.x, texColor2.y, texColor2.z);
      n = normalize(norNor);
   }
	e = normalize((vec3(0.0,0.0,0.0) - pos.xyz));
	l1 = normalize(lightPos1 - pos.xyz);
	h1 = normalize(l1 + e);
	light1 = (ka + (kd * max(dot(l1,n), 0.0)) + (ks * pow(max(dot(h1,n), 0.0), s))) * intensity;
	intensity = 0.6;
	color = light1;

	if (terrainToggle == 1)
	{
		vec4 texColor1 = texture2D(uTexUnit, vTexCoord);
      color.r = min(color.r, 0.5);
      color.g = min(color.g, 0.5);
      color.b = min(color.b, 0.5);
		gl_FragColor = texColor1 * 2.0 * vec4(color.r, color.g, color.b, 1.0);
	}
	else // this is the worst if statement
	{
      // we will never enter this. the toggle is never untoggled
		gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
		gl_FragColor = vec4(1, 1, 1, 1.0);
	}
}
