#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragPos = vec3(model * vec4(pos, 1.0));
	fragNormal = mat3(transpose(inverse(model))) * normal;
	gl_Position = projection * view * vec4(fragPos, 1.0);
}