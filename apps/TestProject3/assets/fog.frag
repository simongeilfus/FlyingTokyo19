uniform vec3        uFogColor;
uniform float       uFogDensity;

uniform vec3        uSunColor;
uniform vec3        uSunDirection;
uniform float       uSunDispertion;
uniform float       uSunIntensity;
uniform vec3        uInscatteringCoeffs;


// Fog Functions from Iñigo Quilez
// http://www.iquilezles.org/www/articles/fog/fog.htm
vec3 applyFog(  in vec3  rgb,      // original color of the pixel
              in float dist,    // camera to point distance
              in vec3  rayOri,   // camera position
              in vec3  rayDir ) // camera to point vector
{
    float minSc         = 0.02;
    float density       = -(dist+10.0) * uFogDensity * 0.5 - dist * 0.0025;
    float sunAmount     = pow( max( dot( rayDir, uSunDirection ), 0.0 ), 1.0/(0.008+uSunDispertion*10.0) );
    vec3 sunFogColor    = mix( uFogColor, uSunColor, uSunIntensity * 10.0 * pow(sunAmount,10.0) );
    vec3 insColor       = vec3(1.0) - clamp( vec3(
                                      exp(density*(uInscatteringCoeffs.x+minSc)),
                                      exp(density*(uInscatteringCoeffs.y+minSc)),
                                      exp(density*(uInscatteringCoeffs.z+minSc)) ),
                                vec3(0), vec3(1) );

    return mix( rgb, sunFogColor, insColor );
}

in vec4     vPosition;
out vec4    oColor;

void main(){
    vec3 rayDir = vPosition.xyz;
    float dist = length( rayDir );
    rayDir /= dist;
    oColor = vec4( applyFog( vec3( 0.0f ), dist, vec3(0), rayDir ), 1.0f );
}