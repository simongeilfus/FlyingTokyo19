out vec4 oColor;

void main(){
	vec2 uv = gl_PointCoord.xy;
	float circle = smoothstep( 0.5, 0.4, length( uv - vec2(0.5f) ) );
    oColor = vec4( circle );
}