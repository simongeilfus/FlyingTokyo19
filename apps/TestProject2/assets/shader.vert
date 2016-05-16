uniform mat4    ciModelViewProjection;
in vec4         ciPosition;

void main(){
	vec4 pos 	= ciPosition;
	pos.z 		= 0.0f;
	gl_PointSize = ciPosition.z;
    gl_Position	= ciModelViewProjection * pos;
}