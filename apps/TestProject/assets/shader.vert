uniform mat4    ciModelViewProjection;

in vec4         ciPosition;
in vec4 		ciColor;

uniform float horizontalPosition;

out vec4 vColor;

void main(){
	vColor 		= ciColor;
	vec4 pos 	= ciPosition;
	pos.x 		+= horizontalPosition;
    gl_Position	= ciModelViewProjection * pos;
}