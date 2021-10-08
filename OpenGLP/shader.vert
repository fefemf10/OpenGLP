#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexUV;

out vec3 UV;
out vec3 vColor;

layout(std140, binding = 0) uniform BufferTransform
{
	mat4 projection;
	mat4 view;
};

void main()
{
	gl_Position = projection * view * vec4(position, 1.0);
	vColor = vertexColor;
	UV = vertexUV;
}