#version 400

struct Light {
	vec4 pos;
	float range;
};

uniform sampler2D colorTextureSampler;

uniform mat4 MVP;
uniform Light principalLight;
uniform Light secondaryLight;

in vec4 gPosition;
in vec2 gTexCoord;
in vec3 gNormal;


void main() {
	float dist1 = distance(principalLight.pos, gPosition);
	float dist2 = distance(secondaryLight.pos, gPosition);

	float light1Contrib = 1.0 - dist1 / (principalLight.range * 1.5);
	float light2Contrib = 1.0 - dist2 / (secondaryLight.range * 1.5);

	light1Contrib = clamp(light1Contrib, 0.0, 1.0);
	light2Contrib = clamp(light2Contrib, 0.0, 1.0);

	light1Contrib *= dot(normalize(vec3(principalLight.pos - gPosition)), gNormal);
	light2Contrib *= dot(normalize(vec3(secondaryLight.pos - gPosition)), gNormal);

	float totalIlum = clamp(light1Contrib + light2Contrib, 0.0, 1.0);

	vec3 color = texture(colorTextureSampler, gTexCoord).rgb;
	vec3 finalColor = color * totalIlum;

	gl_FragColor = vec4(finalColor, 1.0);
}