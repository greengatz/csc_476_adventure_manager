varying vec2 fragTexCoords;
uniform sampler2D tex;
uniform vec4 color;

void main()
{
	// float alpha = texture2D(tex, fragTexCoords).r;
	// gl_FragColor = vec4(color.r, color.g, color.b, color.w * alpha + .05);

	vec4 texture = texture2D(tex, fragTexCoords);
	// gl_FragColor = vec4(color.x, color.y, color.z, texture.w * color.w); //only orange
	// gl_FragColor = vec4(texture.x * color.x, color.y, color.z, color.w);
	vec4 col = vec4(texture.x * color.x, texture.y * color.y, texture.z * color.z, color.w);

	if (texture.x < 0.15 && texture.y < 0.15 && texture.z < 0.15) {
		discard;
	}
	// if (texture.x < 0.15 && texture.y < 0.15 && texture.z < 0.15) {
	// if (col.x > 0.98 && col.y > 0.98 && col.z > 0.98) {
	// 	discard;
	// }

	gl_FragColor = col;
}
