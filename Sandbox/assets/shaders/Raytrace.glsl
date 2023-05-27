#type vertex
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;


void main()
{
    gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout (location = 0) out vec4 o_Color;

uniform vec2 u_ScreenSize;
uniform mat4 u_CameraView;
uniform mat4 u_CameraProjection;
uniform float u_Time;

layout (std430, binding = 0) readonly buffer Octree
{
    uint voxels[];
};

#define EPSILON 0.01
#define SAMPLES 2

const vec3 POS[8] = {
vec3(1, -1, -1),
vec3(1, -1, 1),
vec3(1, 1, -1),
vec3(1, 1, 1),
vec3(-1, -1, -1),
vec3(-1, -1, 1),
vec3(-1, 1, -1),
vec3(-1, 1, 1)
};

struct Ray
{
	vec3 o;
	vec3 d;
	vec3 invDir;
};

struct Hit
{
	vec3 p;
	float tmin;
	float tmax;
	vec3 minBox;
	vec3 maxBox;
};

bool BBoxIntersect(const vec3 boxMin, const vec3 boxMax, const Ray r, out float t0, out float t1) {
	vec3 tbot = r.invDir * (boxMin - r.o);
	vec3 ttop = r.invDir * (boxMax - r.o);
	vec3 tmin = min(ttop, tbot);
	vec3 tmax = max(ttop, tbot);
	vec2 t = max(tmin.xx, tmin.yz);
	t0 = max(t.x, t.y);
	t = min(tmax.xx, tmax.yz);
	t1 = min(t.x, t.y);

	return t1 > max(t0, 0.0);
}

vec3 CalculateBoxNormal(const vec3 hitPoint, const vec3 boxMin, const vec3 boxMax)
{
	vec3 normal;
	vec3 boxCenter = (boxMin + boxMax) * 0.5;
    vec3 toCenter = boxCenter - hitPoint;
    vec3 boxHalfExtents = (boxMax - boxMin) * 0.5;
    vec3 normalizedToCenter = normalize(toCenter);
    vec3 absNormalizedToCenter = abs(normalizedToCenter);
    int maxAxis = 0;
    float maxComponent = absNormalizedToCenter.x;

    if (absNormalizedToCenter.y > maxComponent)
    {
        maxAxis = 1;
        maxComponent = absNormalizedToCenter.y;
    }

    if (absNormalizedToCenter.z > maxComponent)
    {
        maxAxis = 2;
        maxComponent = absNormalizedToCenter.z;
    }
    normal = vec3(maxAxis == 0 ? normalizedToCenter.x : 0.0,
              maxAxis == 1 ? normalizedToCenter.y : 0.0,
              maxAxis == 2 ? normalizedToCenter.z : 0.0);
	return normalize(normal);
}

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

uint[8] OrderOctantsByDistance(vec3 rayOrigin, vec3 parentCenter, float scale) {
    const vec3 POS[8] = {
        vec3(1, -1, -1),
        vec3(1, -1, 1),
        vec3(1, 1, -1),
        vec3(1, 1, 1),
        vec3(-1, -1, -1),
        vec3(-1, -1, 1),
        vec3(-1, 1, -1),
        vec3(-1, 1, 1)
    };

    uint[8] indices = uint[8](0, 1, 2, 3, 4, 5, 6, 7);
    float distancesSquared[8];

    for (uint i = 0; i < 8; i++) {
        vec3 diff = parentCenter + POS[i] * scale - rayOrigin;
        distancesSquared[i] = dot(diff, diff);
    }

    // Shell sort the indices based on distances squared
    uint gap = 4; // 8 / 2
    while (gap > 0) {
        for (uint i = gap; i < 8; i++) {
            float tempDist = distancesSquared[indices[i]];
            uint tempIndex = indices[i];
            int j = int(i) - int(gap);
            while (j >= 0 && distancesSquared[indices[j]] > tempDist) {
                indices[uint(j + gap)] = indices[uint(j)];
                j -= int(gap);
            }
            indices[uint(j + gap)] = tempIndex;
        }
        gap /= 2;
    }

    return indices;
}

vec4 trace(Ray ray, inout Hit hit) {
    hit.tmin = -1.0;
	vec3 center = vec3(0.0);
    float scale = 1.0;
	vec3 minBox = center - scale;
	vec3 maxBox = center + scale;
	vec4 f = vec4(0.0);
    struct Stack {
		uint index;
		vec3 center;
		float scale;
	};
    Stack stack[15];
    int stackPos = 1;
    float t0;
    float t1;
    if (!BBoxIntersect(minBox, maxBox, ray, t0, t1)) return f;
    uint index = 0u;
    scale *= 0.5;
    stack[0] = Stack( 0u, center, scale);

    while(stackPos-- > 0) {
        center = stack[stackPos].center;
		index = stack[stackPos].index;
		scale = stack[stackPos].scale;
        uint voxel_node = voxels[index];
        uint voxel_group_offset = voxel_node >> 16;
        uint voxel_child_mask = (voxel_node & 0x0000FF00u) >> 8u;
        uint voxel_leaf_mask = voxel_node & 0x000000FFu;

        uint order[8] = uint[8](0, 1, 2, 3, 4, 5, 6, 7);
        
        for (uint i = 0u; i < 8u; ++i) {
            uint loc = order[i];

            bool empty = (voxel_child_mask & (1u << (7 - loc))) == 0u;
            bool is_leaf = (voxel_leaf_mask & (1u << (7 - loc))) != 0u;
            if (empty){ //empty
                continue;
            }
            
            vec3 new_center = center + scale * POS[loc];
            vec3 minBox = new_center - scale;
            vec3 maxBox = new_center + scale;
            
            float tmin;
            float tmax;

            if (!BBoxIntersect(minBox, maxBox, ray, tmin, tmax)){
                continue;
            }
            if (is_leaf){ //not empty, but a leaf
                if (hit.tmin == -1 || hit.tmin > tmin)
                {
                    uint col = voxels[voxel_group_offset+loc];
                    f = vec4(unpackUnorm4x8(col).xyz, 1.0);
                    hit.tmin = tmin;
                    hit.tmax = tmax;
					hit.minBox = minBox;
					hit.maxBox = maxBox;
                }
            } else { //not empty and not a leaf
            	stack[stackPos++] = Stack(voxel_group_offset+loc, new_center, scale*0.5);
            }
        }
    }

    hit.p = ray.o + ray.d * hit.tmin*0.999;

    return f;
}

void main()
{
	vec2 uv = (gl_FragCoord.xy / u_ScreenSize) * 2.0 - 1.0;

    // Jitter for TAA
    uint jitterSeed = uint(u_Time*1000.0);
    uv += RandomDirection(jitterSeed).xy*0.001;

	vec4 rayClip = vec4(uv, -1.0, 1.0);
	vec4 rayEye = inverse(u_CameraProjection) * rayClip;
	rayEye = vec4(rayEye.xy, -1.0, 0.0);
	vec3 rayWorld = (inverse(u_CameraView) * rayEye).xyz;

    uint randomState = int((gl_FragCoord.y * u_ScreenSize.x + gl_FragCoord.x) * u_Time);

	vec3 rayDir = normalize(rayWorld); 
	vec3 rayOrigin = vec3(inverse(u_CameraView)[3]);

	Ray ray;
	Hit hit;

	ray.o = rayOrigin;
	ray.d = rayDir;
	ray.invDir = 1.0/rayDir;

	vec4 color = trace(ray, hit);

    if (color.a < EPSILON) discard;

    vec3 normal = CalculateBoxNormal(hit.p, hit.minBox, hit.maxBox);

	Ray aoRay;
    Hit aoHit;

    float ao = 0;

    for (int i = 0; i < SAMPLES; i++)
    {
        aoRay.o = hit.p;

        vec3 aoDir = RandomDirection(randomState);
        if (dot(aoDir, normal) >= 0) aoDir *= -1;

        aoRay.d = aoDir;
        aoRay.invDir = 1.0/aoDir;

        vec4 aoResult = trace(aoRay, aoHit);
    
        if (aoResult.a < EPSILON) ao += 1;
    }

    ao /= SAMPLES;

    color *= ao;


    o_Color = vec4(color.rgb, 1.0);
}