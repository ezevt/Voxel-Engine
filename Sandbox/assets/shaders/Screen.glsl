#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;


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

struct Ray
{
	vec3 o;
	vec3 d;
	vec3 invDir;
};

struct Hit
{
	vec3 p;
	vec3 n;
	float t;
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

void main()
{
	vec2 uv = (gl_FragCoord.xy / u_ScreenSize) * 2.0 - 1.0;

	vec4 rayClip = vec4(uv, -1.0, 1.0);
	vec4 rayEye = inverse(u_CameraProjection) * rayClip;
	rayEye = vec4(rayEye.xy, -1.0, 0.0);
	vec3 rayWorld = (inverse(u_CameraView) * rayEye).xyz;

	vec3 rayDir = normalize(rayWorld); 
	vec3 rayOrigin = vec3(inverse(u_CameraView)[3]);

	Ray ray;
	Hit hit;

	ray.o = rayOrigin;
	ray.d = rayDir;
	ray.invDir = 1.0/rayDir;

	vec3 color = vec3(0.0);
	if (BBoxIntersect(vec3(-1.0), vec3(1.0), ray, hit))
	{
		color = vec3(1.0);
	}

    o_Color = vec4(color, 1.0);
}