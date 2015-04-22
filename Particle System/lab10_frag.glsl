varying vec2 fragTexCoords;
uniform sampler2D alphaTexture;
uniform vec4 color;

void main()
{
	float alpha = texture2D(alphaTexture, fragTexCoords).r;
	gl_FragColor = vec4(color.rgb, color.a*alpha);
}
