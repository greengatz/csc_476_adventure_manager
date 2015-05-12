#version 120

varying vec4 color; // passed from the vertex shader
uniform float alphaValue;

void main()
{
	gl_FragColor = vec4(color.r, color.g, color.b, alphaValue);
}
