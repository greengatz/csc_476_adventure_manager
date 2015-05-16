attribute vec4 vertPosition;
attribute vec2 vertTexCoords;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform float scale;
varying vec2 fragTexCoords;

void main()
{
	// Billboarding: set the upper 3x3 to be the identity matrix
	// mat4 MV0 = MV;
	// MV0[0] = vec4(1.0, 0.0, 0.0, 0.0);
	// MV0[1] = vec4(0.0, 1.0, 0.0, 0.0);
	// MV0[2] = vec4(0.0, 0.0, 1.0, 0.0);
	// gl_Position = P * MV0 * vec4(scale * vertPosition.xy, 0.0, 1.0);

	// vec3 pos = M * V * vertPosition;
	gl_Position = P * V * M * vec4(vertPosition.x, vertPosition.y, vertPosition.z, 1.0);
	fragTexCoords = vertTexCoords;
}