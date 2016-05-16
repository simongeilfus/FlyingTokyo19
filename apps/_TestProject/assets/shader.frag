out vec4 oColor;
in vec2	vUv;
void main(){
    oColor = vec4( vec3( cos( 10.0f * vUv.x ) ), 1.0f );
}