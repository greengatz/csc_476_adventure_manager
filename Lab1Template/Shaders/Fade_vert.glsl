#version 120
attribute vec4 vertPos;

uniform mat4 uModelMatrix;
uniform float alphaValue;

varying vec4 color; // Pass to fragment shader

void main()
{
	gl_Position = uModelMatrix * vertPos;
	//Fade To Black
	color = vec4(0.0, 0.0, 0.0, 0.0);
}
