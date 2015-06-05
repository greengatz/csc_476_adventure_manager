varying vec2 fragTexCoords;
// varying float edgeFade;
uniform sampler2D tex;
uniform vec4 color;
uniform float scale;

void main()
{
	vec2 center = vec2(scale / 2.0, scale / 2.0);
	float maxDist = distance(center, vec2(scale, scale));
	float distToPoint = distance(center, fragTexCoords);
	float edgeFade = distToPoint / maxDist;

	vec4 texture = texture2D(tex, fragTexCoords);
	vec4 col = vec4(texture.x * color.x, texture.y * color.y, texture.z * color.z, color.w * edgeFade);

	if (texture.x < 0.15 && texture.y < 0.15 && texture.z < 0.15) {
		discard;
	}

	gl_FragColor = col;
}
