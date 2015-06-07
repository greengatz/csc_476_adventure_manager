attribute vec4 vertPosition;
attribute vec2 vertTexCoords;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform float scale;
varying vec2 fragTexCoords;
// varying float edgeFade;

void main()
{
	gl_Position = P * V * M * vec4(vertPosition.x, vertPosition.y, vertPosition.z, 1.0);
	fragTexCoords = vertTexCoords;

	//calculates the distance to the edge to try to fade it
	// vec2 center = vec2(scale / 2.0, scale / 2.0);
	// float maxDist = distance(center, vec2(scale, scale));
	// float distToPoint = distance(center, vertTexCoords);
	// edgeFade = distToPoint / maxDist;
}