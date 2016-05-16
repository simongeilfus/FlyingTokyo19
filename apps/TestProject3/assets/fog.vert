uniform mat4    ciModelView;
uniform mat4    ciProjectionMatrix;
in vec4         ciPosition;
out vec4        vPosition;
void main(){
    vec4 position   = ciModelView * ciPosition;
    vPosition       = position;
    gl_Position     = ciProjectionMatrix * position;
}