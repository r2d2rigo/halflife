uniform vec3 CameraPos;
uniform vec3 AmbientCube[6];

varying vec3 normal;
varying vec3 _reflection;

varying vec3 _ambientColor;

vec3 AmbientLight(in vec3 worldNormal)
{
	vec3 nSquared = worldNormal * worldNormal;
	ivec3 isNegative = ivec3(worldNormal.x < 0.0, worldNormal.y < 0.0, worldNormal.z < 0.0);
	
	vec3 linearColor;
	linearColor = nSquared.x * AmbientCube[isNegative.x] +
				  nSquared.y * AmbientCube[isNegative.y+2] +
				  nSquared.z * AmbientCube[isNegative.z+4];

	return linearColor;
} 

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	normal = normalize(gl_Normal.xyz);
	vec3 viewDir = normalize(CameraPos - gl_Vertex.xyz);
	_reflection = reflect(viewDir, normal);	
	
	_ambientColor = AmbientLight(gl_Normal.xyz);
	
	gl_Position = ftransform();
}

