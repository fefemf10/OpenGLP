#version 460 core

in vec3 UV;
in vec3 vColor;
out vec4 color;

uniform sampler2DArray textureSampler;

void main()
{
	vec4 tmpColor = texture(textureSampler, UV) * vec4(vColor, 1.0);
	if (tmpColor.a < 0.1)
		discard;
	color = tmpColor;
}