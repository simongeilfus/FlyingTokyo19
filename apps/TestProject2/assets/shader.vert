uniform mat4    ciModelViewProjection;
in vec4         ciPosition;

void main(){
	vec4 pos 	= ciPosition;
    gl_Position	= ciModelViewProjection * pos;
}