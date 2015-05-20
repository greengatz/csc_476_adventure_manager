#version 120

uniform sampler2D gShadowMap;

varying vec2 TexCoordOut;

void main()
{
	float depth = texture2D(gShadowMap, TexCoordOut).x;
	depth = 1.0 - (1.0 - depth) * 25.0;

	gl_FragColor = vec4(depth);
}