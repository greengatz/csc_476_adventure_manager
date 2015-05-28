#version 120

//Texture
uniform sampler2D uTexUnit;

uniform vec4 uLight;
uniform int leafToggle;

varying vec4 point;
varying vec2 vTexCoord;

varying float fogDist;

void shadowFragment(void)
{
	float dist = distance(uLight.xyz, point.xyz / point.w);

	float intensity = (uLight.w - dist) / uLight.w;

	if (intensity <= 0.0)
	{
		discard;
	}

	gl_FragColor.a = min(1.0, intensity);
}

void main()
{
   vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
   // float fogFactor = (5 - fogDist)/(5 - 8); //linear fog equation
   float fogFactor = 1.0 - 1.0 / exp(fogDist * 0.15); //fog density is 0.15
   fogFactor = clamp(fogFactor, 0.0, 1.0);

   vec4 targetColor = vec4(0.0, 0.0, 0.0, 1.0);
   gl_FragColor = mix(targetColor, fogColor, fogFactor);

   if (leafToggle == 1)
   {
   	vec4 texColor = texture2D(uTexUnit, vTexCoord);
   	if (texColor.y > 0.7)
   	{
      	discard;
   	}
   }

   shadowFragment();
}
