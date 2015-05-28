#version 120

uniform vec4 uLight;

varying vec4 point;

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

   shadowFragment();
}
