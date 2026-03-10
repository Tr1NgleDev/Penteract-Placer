#version 450 core

struct vec5
{
	vec4 abcd;
	float e;
};
vec5 add(in vec5 a, in vec5 b) { return vec5(a.abcd + b.abcd, a.e + b.e); }
vec5 sub(in vec5 a, in vec5 b) { return vec5(a.abcd - b.abcd, a.e - b.e); }
vec5 mul(in vec5 a, in vec5 b) { return vec5(a.abcd * b.abcd, a.e * b.e); }
vec5 div(in vec5 a, in vec5 b) { return vec5(a.abcd / b.abcd, a.e / b.e); }
vec5 add(in vec5 a, in float b) { return vec5(a.abcd + vec4(b), a.e + b); }
vec5 sub(in vec5 a, in float b) { return vec5(a.abcd - vec4(b), a.e - b); }
vec5 mul(in vec5 a, in float b) { return vec5(a.abcd * vec4(b), a.e * b); }
vec5 div(in vec5 a, in float b) { return vec5(a.abcd / vec4(b), a.e / b); }
float dot5(in vec5 a, in vec5 b) { return dot(a.abcd, b.abcd) + dot(a.e, b.e); }
float length5(in vec5 v) { return sqrt(dot5(v, v)); }
vec5 normalize5(in vec5 v) { return div(v, sqrt(dot5(v, v))); }
vec4 abcd(in vec5 v) { return v.abcd; }
vec4 bcde(in vec5 v) { return vec4(v.abcd.yzw, v.e); }

out vec4 color;

in vec2 uv;
uniform vec4 screenSize; // w, h, 1/w, 1/h
uniform float time;
struct Camera
{
	vec5 pos;
	vec5 up;
	vec5 forward;
	vec5 left;
	vec5 over;
	vec5 yonder;
	float vFov;
};
layout(std430, binding = 0) buffer CameraBuf
{
	Camera cam;
};
uniform vec5 lightDir;

vec3 sky5D(in vec5 rd)
{
	float y = rd.abcd.x;
	float ay = abs(y);

	if (ay < 1e-6)
		return vec3(0.0);

	float planeY = (y > 0.0) ? 1.0 : -1.0;

	float t = planeY / y;

	vec5 p5 = mul(rd, t);

	vec4 p = bcde(p5);

	const float cellSize   = 1.0;
	const float lineWidth  = 0.025;
	const float fogDensity = 0.25;

	vec4 q = p / cellSize + vec4(0.5);

	vec4 a = abs(fract(q) - 0.5);

	float edge = max(max(a.x, a.y), max(a.z, a.w));

	float w = lineWidth / cellSize;
	float line = step(0.5 - w, edge);

	vec3 col = mix(vec3(1.0), vec3(0.2), line);

	float fog = exp(-fogDensity * (t - 1.0));

	float horizonFade = smoothstep(0.0, 0.08, ay);

	col *= fog * horizonFade;

	if (y > 0.0) col *= 0.85;

	return col;
}

void main()
{
	float aspect = screenSize.x * screenSize.w;
	vec5 ro = cam.pos;
	vec2 ndc = uv * 2.0 - 1.0;
	float tanHalfFov = tan(cam.vFov * 0.5);
	float px = ndc.x * tanHalfFov * aspect;
	float py = -ndc.y * tanHalfFov;
	vec5 rd = normalize5(
		add(
			add(cam.forward, mul(cam.left, -px)),
			mul(cam.up, -py)
		)
	);

	color = vec4(rd.abcd.xyz, 1.0); // ???
	//color = vec4(uv, 0.0, 1.0);
	color = vec4(sky5D(rd), 1.0);
}