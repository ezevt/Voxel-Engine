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

layout(binding = 1) uniform JitterSettings
{
	vec2 PreviousJitter;
	vec2 CurrentJitter;
    float JitterScale;
};

uint NextRandom(inout uint state)
{
	state = state * 747796405 + 2891336453;
	uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
	result = (result >> 22) ^ result;
	return result;
}

float RandomValue(inout uint state)
{
	return NextRandom(state) / 4294967295.0; // 2^32 - 1
}

float RandomValueNormalDistribution(inout uint state)
{
	float theta = 2 * 3.1415926 * RandomValue(state);
	float rho = sqrt(-2 * log(RandomValue(state)));
	return rho * cos(theta);
}

vec3 RandomDirection(inout uint state)
{
	float x = RandomValueNormalDistribution(state);
	float y = RandomValueNormalDistribution(state);
	float z = RandomValueNormalDistribution(state);
	return normalize(vec3(x, y, z));
}

uniform sampler2DMS u_ColorTextureMS;
uniform sampler2D u_ColorTexture;

const float feedbackFactor = 0.5; // Weight of the previous frame
const float sigma = 1;
const float PI = 3.1415926535897932384626433832795;

float gaussian(float x, float sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * PI) * sigma);
}

vec4 GetPreviusTextureBlured(vec2 uv)
{
    vec2 texelSize = 1.0 / ScreenSize;
    vec4 color = vec4(0.0);

    // Compute the Gaussian kernel weights
    int kernelSize = int(sigma * 6.0 + 1.0);
    int halfSize = kernelSize / 2;
    float totalWeight = 0.0;

    for (int i = -halfSize; i <= halfSize; ++i)
    {
        float offset = float(i);
        float weight = gaussian(offset, sigma);
        color += texture(u_ColorTexture, uv + offset * texelSize) * weight;
        totalWeight += weight;
    }

    return color;
}

vec4 BilateralFilter(in vec2 texCoord)
{
    const int kernelSize = 1;

    vec4 center = texture(u_ColorTexture, texCoord).rgba;
    vec4 result = vec4(0.0);
    float normalization = 0.0;

    // Iterate over the kernel
    for (int i = -kernelSize; i <= kernelSize; ++i)
    {
        for (int j = -kernelSize; j <= kernelSize; ++j)
        {
            vec2 offset = vec2(float(i), float(j));
            vec2 neighborTexCoord = texCoord + offset / ScreenSize;

            vec4 neighborColor = texture(u_ColorTexture, neighborTexCoord).rgba;

            // Compute the spatial and range weights
            float spatialWeight = exp(-(i * i + j * j) / (2.0 * kernelSize * kernelSize));
            float rangeWeight = exp(-pow(length(neighborColor - center), 2.0) / (2.0 * kernelSize * kernelSize));

            // Accumulate the result
            result += neighborColor * spatialWeight * rangeWeight;
            normalization += spatialWeight * rangeWeight;
        }
    }

    // Normalize the result
    result /= normalization;

    return result;
}

void main()
{
    // TODO: Denoise

    vec2 jitter = (CurrentJitter-PreviousJitter)*JitterScale;

    vec4 currentColor = texelFetch(u_ColorTextureMS, ivec2((o_UV + jitter)*ScreenSize), 0);

    vec4 previousColor = texture(u_ColorTexture, o_UV);

    vec4 taaColor = mix(currentColor, previousColor, feedbackFactor);

    o_Color = taaColor;
}