out vec4 oColor;
void main(){
	vec2 uv = gl_PointCoord.xy;
	float dist = pow( 1.0f - length( uv - vec2( 0.5f ) ), 3.0f );
    oColor = vec4( vec3(dist), 0.6f * dist );
}