#version 430 core

layout (location = 0) in vec2 aPos;

uniform float aspectRatioV;
out vec2 vPosition;


vec4 scaleView (float ratio) {
		return ( int(ratio > 1 ) * vec4( 1.0f/ratio, 1.0f, 1.0f, 1.0f )
		       + int(ratio <=1 ) * vec4( 1.0f, 1.0f*ratio, 1.0f, 1.0f ));
}

void main() {
	vPosition = aPos;

	gl_Position = vec4(aPos*2,0,1);
}