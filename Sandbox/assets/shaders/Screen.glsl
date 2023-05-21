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

out vec4 o_Color;

uniform vec2 u_ScreenSize;
uniform mat4 u_CameraView;
uniform mat4 u_CameraProjection;

layout (std430, binding = 0) readonly buffer Octree
{
    uint voxels[];
};

//uint voxels[230] = uint[230](66046u,196352u,719365u,1111306u,1504267u,1798287u,2010960u,2383776u,2760672u,3081212u,3244159u,3317855u,3448927u,3604224u,4127237u,4521728u,4997311u,5075087u,5308160u,5821483u,6094592u,6583690u,6924885u,7205389u,7533324u,7879365u,8190469u,8581900u,8917486u,9045243u,9153109u,9436928u,9961216u,10485504u,10945277u,11012853u,11206400u,11685298u,11992832u,12474280u,12782835u,12914672u,13238016u,13729664u,14169036u,14418430u,14484221u,14565312u,14946798u,33023u,57599u,12543u,52479u,41727u,51455u,52479u,8447u,65535u,52479u,52479u,65535u,65535u,57599u,61695u,35071u,65535u,43775u,65535u,61695u,65535u,62207u,65535u,65535u,65535u,65535u,65535u,49407u,61695u,20735u,12543u,52479u,50431u,65535u,65535u,52479u,52479u,65535u,65535u,20735u,50431u,65535u,54783u,65535u,65535u,65535u,65535u,65535u,65535u,65535u,65535u,62975u,28927u,65535u,12543u,65535u,33023u,61695u,56831u,14335u,12543u,60159u,65535u,47871u,65535u,61695u,61695u,65535u,65535u,29695u,12543u,65535u,511u,8447u,47103u,49407u,63743u,65535u,19711u,8959u,1535u,61695u,61695u,65535u,65535u,13311u,12799u,4351u,30719u,5631u,35071u,35071u,35071u,767u,52479u,52479u,65535u,65535u,52479u,52479u,8191u,49151u,35583u,65535u,44031u,65535u,2303u,36863u,2815u,45055u,65535u,65535u,65535u,65535u,53247u,61439u,65535u,65535u,3327u,2815u,767u,52479u,52479u,65535u,65535u,52479u,52479u,65535u,65535u,52479u,65535u,56831u,19967u,65535u,65535u,65535u,65535u,65535u,65535u,65535u,65535u,65535u,13311u,65535u,24575u,24575u,3327u,1279u,4095u,3583u,4095u,511u,61183u,65535u,65535u,65535u,61183u,57343u,49151u,32767u,65535u,65535u,22527u,1023u,4095u,4095u,511u,13311u,13311u,767u,1023u,4607u,1279u,65535u,65535u,13311u,13311u,16383u,8191u,30719u,511u);
//uint voxels[2] = uint[2](114816u, 139296u);

const vec3 PPP = vec3(1, 1, 1);
const vec3 PNP = vec3(1, -1, 1);
const vec3 PNN = vec3(1, -1, -1);
const vec3 NPN = vec3(-1, 1, -1);
const vec3 NNN = vec3(-1, -1, -1);
const vec3 NNP = vec3(-1, -1, 1);
const vec3 NPP = vec3(-1, 1, 1);
const vec3 PPN = vec3(1, 1, -1);
const vec3 POS[8] = vec3[8](PNN, PNP, PPN, PPP, NNN, NNP, NPN, NPP);

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
};

bool BBoxIntersect(const vec3 boxMin, const vec3 boxMax, const Ray r, out Hit hit) {
	vec3 tbot = r.invDir * (boxMin - r.o);
	vec3 ttop = r.invDir * (boxMax - r.o);
	vec3 tmin = min(ttop, tbot);
	vec3 tmax = max(ttop, tbot);
	vec2 t = max(tmin.xx, tmin.yz);
	float t0 = max(t.x, t.y);
	t = min(tmax.xx, tmax.yz);
	float t1 = min(t.x, t.y);
	hit.tmin = t0;
	hit.tmax = t1;
	return t1 > max(t0, 0.0);
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

vec4 trace(Ray ray, inout Hit hit) {
	vec3 center = vec3(0.0f);
    float scale = 1.0f;
	vec3 minBox = center - scale;
	vec3 maxBox = center + scale;
	vec4 f = vec4(0.0f);
    struct Stack {
		uint index;
		vec3 center;
		float scale;
	};
    Stack stack[15];
    int stackPos = 1;
    if (!BBoxIntersect(minBox, maxBox, ray, hit)) return f;
    uint index = 0u;
    scale *= 0.5f;
    stack[0] = Stack( 0u, center, scale);
    float minDist = -1;
    int i = 0;
    while(stackPos-- > 0) {
        i++;
        center = stack[stackPos].center;
		index = stack[stackPos].index;
		scale = stack[stackPos].scale;
        uint voxel_node = voxels[index];
        uint voxel_group_offset = voxel_node >> 16;
        uint voxel_child_mask = (voxel_node & 0x0000FF00u) >> 8u;
        uint voxel_leaf_mask = voxel_node & 0x000000FFu;
        uint accumulated_offset = 0u;
        for (uint i = 0u; i < 8u; ++i) {
            bool empty = (voxel_child_mask & (1u << (7 - i))) == 0u;
            bool is_leaf = (voxel_leaf_mask & (1u << (7 - i))) != 0u;
            if (empty){ //empty
                continue;
            }
            
            vec3 new_center = center + scale * POS[i];
            vec3 minBox = new_center - scale;
            vec3 maxBox = new_center + scale;
            

            if (!BBoxIntersect(minBox, maxBox, ray, hit)){
                accumulated_offset+=1u;
                continue;
            }
            if (is_leaf){ //not empty, but a leaf
                if (minDist == -1 || minDist > hit.tmin)
                {
                    uint col = voxels[voxel_group_offset+accumulated_offset];
                    f = vec4(unpackUnorm4x8(col).xyz, 1.0f);
                    minDist = hit.tmin;
                }
                accumulated_offset+=1u;
            } else { //not empty and not a leaf
            	stack[stackPos++] = Stack(voxel_group_offset+accumulated_offset, new_center, scale*0.5f   );
                accumulated_offset+=1u;
            }
        }
    }

    hit.p = ray.o + ray.d*minDist*0.99;

    return f;
}

void main()
{
	vec2 uv = (gl_FragCoord.xy / u_ScreenSize) * 2.0 - 1.0;

	vec4 rayClip = vec4(uv, -1.0, 1.0);
	vec4 rayEye = inverse(u_CameraProjection) * rayClip;
	rayEye = vec4(rayEye.xy, -1.0, 0.0);
	vec3 rayWorld = (inverse(u_CameraView) * rayEye).xyz;

    uint randomState = int(gl_FragCoord.y * u_ScreenSize.x + gl_FragCoord.x);

	vec3 rayDir = normalize(rayWorld); 
	vec3 rayOrigin = vec3(inverse(u_CameraView)[3]);

	Ray ray;
	Hit hit;

	ray.o = rayOrigin;
	ray.d = rayDir;
	ray.invDir = 1.0/rayDir;

	vec4 color = trace(ray, hit);

    if (color.a < 0.01) discard;

    o_Color = vec4(color.rgb, 1.0);
}