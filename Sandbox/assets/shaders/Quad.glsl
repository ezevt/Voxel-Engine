#type vertex
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

layout (location = 0) out vec2 o_UV;

void main()
{
    o_UV = (a_Position.xy+1.0) / 2.0;
    gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

out vec4 o_Color;

layout (location = 0) in vec2 o_UV;

layout (std140, binding = 0) uniform DefaultSettings
{
    uniform mat4 CameraProjection;
    uniform mat4 CameraView;
    uniform vec2 ScreenSize;
    uniform float Time;
};

uniform sampler2D u_ColorTexture;

void main()
{
    // TODO: Denoise

    o_Color = texture(u_ColorTexture, o_UV);
}