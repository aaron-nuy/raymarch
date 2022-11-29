#version 430 core
#define PI 3.14159265359

out vec4 FragColor;
in vec2 vPosition;

uniform vec2 mouse;
uniform vec3 cameraPos;
uniform float fov;
uniform float aspec;
uniform float time;
uniform vec3 sphereloc;
uniform float sphereRadius;
uniform int maxits;
uniform float thresh;
uniform mat4 matrix;



float atan2(in float y, in float x) {
    bool s = (abs(x) > abs(y));
    return mix(PI/2.0 - atan(x,y), atan(y,x), s);
}

float smin(float a, float b, float k) {
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

float smax(float a, float b, float k) {
    return smin(a, b, -k);
}

float sdSphere(vec3 position, vec3 centre, float radius) {
	return length(position-centre) - radius;
}

float sdBox(vec3 position, vec3 origin, vec3 bound) {
	 vec3 d = abs(position-origin) - bound;
	 return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

vec3 hypot(vec3 x, vec3 y) {
	return sqrt(x*x+y*y);
}

float hypot(float x, float y) {
	return sqrt(x*x+y*y);
}

vec3 sphereP = vec3(3,3,3);
float sphereR = 0.1;

vec3 boxP = vec3(2,2.9,3);
vec3 boxB = vec3(0.5);


const vec3 lightP = vec3( 2 , 3 , -1);

int raymarch(vec3 origin, vec3 direction) {

	int i = 0;
	for( i; i<=maxits; i++) {
 
		float m = smin( sdSphere(mod(origin,6),sphereP,sphereR), sdBox(origin,boxP,boxB) , 1.5);
		m = smin(m,m,5.5);
		//m = smin(sdSphere(mod(origin+time/1000,6),sphereloc,sphereRadius),100000, 0.4f);
		origin += direction*m;
		if(m < thresh ) return i;
		
	} 
	return -1;
}

void main() {

	vec3 rayOrigin = cameraPos;
	
	vec3 rayDirection = vec3(vPosition.x*aspec,vPosition.y,-1);
	
	rayDirection = (matrix * vec4(normalize(rayDirection),0)).xyz;
	
	int r = raymarch(rayOrigin, rayDirection.xyz);


	if(r > 0)
		FragColor = vec4(r/float(maxits),0,0,1);
	else
		FragColor = vec4(0,0,0,1);

}