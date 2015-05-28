#version 120

//Texture
uniform sampler2D uTexUnit;

uniform vec4 uLight;
uniform int leafToggle;

varying vec4 point;
varying vec2 vTexCoord;

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
   gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0);

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
