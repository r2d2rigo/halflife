uniform sampler2D texture;
uniform samplerCube cubemap;

// uniform float AmbientLight;
uniform float ShadeLight;
uniform vec3 LightDir;
uniform vec3 LightColor;
uniform float CubemapAmount;

varying vec3 normal;
varying vec3 _reflection;
varying vec3 _ambientColor;

void main (void)  
{     
	float lightCos = dot(normal, LightDir);

	lightCos = clamp(lightCos, -1, 1);

	float lightValue = ShadeLight;

	float r = 1.5;

	lightCos = (lightCos + (r - 1.0)) / r;
	if (lightCos > 0.0)
	{
		lightValue -= ShadeLight * lightCos;
	}
	
	vec3 finalLightColor = _ambientColor + LightColor * lightValue;
	
	vec4 cubemapColor = textureCube(cubemap, _reflection);
	vec4 finalColor = texture2D(texture, gl_TexCoord[0].st);
	finalColor.rgb *= finalLightColor;
	finalColor.rgb += cubemapColor.rgb * CubemapAmount;
	// finalColor.rgb = cubemapColor.rgb;
	
	gl_FragColor = finalColor;
}