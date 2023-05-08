#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

uniform mat4 u_CameraMatrix;

void main()
{
    gl_Position = u_CameraMatrix * vec4(a_Position, 1.0);
}

#type fragment

#version 460 core

out vec4 o_Color;

void main()
{
	// TODO: dynamic resolution
	vec2 screenPos = (gl_FragCoord.xy/vec2(800, 600)) * 2.0 - 1.0;

    o_Color = vec4(1.0, 1.0, 1.0, 1.0);
}