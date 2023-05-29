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

float sigmaSpatial = 0.01;
float sigmaIntensity = 0.01;

void main()
{
    vec2 texelSize = 1.0 / ScreenSize;
    vec3 centerPixel = texture(u_ColorTexture, o_UV).rgb;
    vec3 outputColor = vec3(0.0);
    float totalWeight = 0.0;

    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            vec2 offset = vec2(float(i), float(j)) * texelSize;
            vec3 currentPixel = texture(u_ColorTexture, o_UV + offset).rgb;
            
            float spatialDistance = length(vec2(i, j)) * texelSize.x;
            float intensityDistance = length(centerPixel - currentPixel);
            
            float weight = exp(-(spatialDistance * spatialDistance) / (2.0 * sigmaSpatial * sigmaSpatial));
            weight *= exp(-(intensityDistance * intensityDistance) / (2.0 * sigmaIntensity * sigmaIntensity));
            
            outputColor += currentPixel * weight;
            totalWeight += weight;
        }
    }

    o_Color = vec4(outputColor.rgb/totalWeight, 1.0);
}