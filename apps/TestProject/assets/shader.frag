
in vec4 vColor;

out vec4 oColor;

uniform vec4 colorModifier;

void main(){
	vec2 uv = gl_PointCoord.xy;
	float dist = pow( 1.0f - length( uv - vec2( 0.5f ) ), 3.0f );
    oColor = vec4( dist * vColor * colorModifier );
}