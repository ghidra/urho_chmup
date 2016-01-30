#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vTexCoord2;
varying vec4 vTexCoord4;

varying vec3 vNormal;
varying vec4 vWorldPos;
varying vec4 vIPos;

#ifdef COMPILEVS
uniform vec3 cDirection;
uniform float cSpeed;
//uniform sampler2D sNormalMap;
#endif

//#ifdef COMPILEPS
//https://www.shadertoy.com/view/4sfGzS
//https://www.shadertoy.com/view/MdfGRX#

#ifdef DIFFMAP

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
    
    vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
    vec2 rg = texture2D( sNormalMap, (uv+0.5)/256.0, -100.0 ).yx;//this function was not available on the laptop
        //vec2 rg = texture2D( sNormalMap, (uv+0.5)/256.0 ).yx;
    return mix( rg.x, rg.y, f.z );
}

#else

float hash( float n ) { return fract(sin(n)*753.5453123); }
float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
  
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}

#endif

const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 );
float fbm( in vec3 x )
{
	float f = 0.0;
	vec3 q = 8.0*x;
    f  = 0.5000*noise( q ); q = m*q*2.01;
    f += 0.2500*noise( q ); q = m*q*2.02;
    f += 0.1250*noise( q ); q = m*q*2.03;
    f += 0.0625*noise( q ); q = m*q*2.01;

    return f*1.2;
}

float bias(float t, float b)
{
    return (t / ((((1.0/b) - 2.0)*(1.0 - t))+1.0));
}

#ifdef GAIN
float gain(float t,float g)
{
    if(t < 0.5)
    {     
        return bias(t * 2.0,g)/2.0;   
    }else{
        return bias(t * 2.0 - 1.0,1.0 - g)/2.0 + 0.5; 
    }
}
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;

    //modify positions
    float n = fbm((iPos.xyz*0.5)+vec3(0.0,-cElapsedTime,0.0));
    vec3 disp = iNormal*n;
    //get the dot of normal and direction
    float d = dot( iNormal, -cDirection );//just get the dot
    float cd = bias( clamp(d,0.0,1.0), 0.15 )*n;//this limits the dot deformation and mults noise back it so it goes up and down
    vec3 worldPos = ((iPos+(vec4(disp,0.0)*cd*clamp(cSpeed,100.0,200.0)*0.025)) * modelMatrix).xyz;//limit speed to 200 otherwise deformation is insane

    //vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);

    vTexCoord2 = iTexCoord;

    vNormal = GetWorldNormal(modelMatrix);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));
    //vIPos = iPos;//this comes from Transform.glsl
    vIPos = (iPos*vec4(1.0,0.2,1.0,1.0))+vec4(0.0,-cElapsedTime,0.0,0.0);//usetime

    vec3 tangent = GetWorldTangent(modelMatrix);
    vec3 bitangent = cross(tangent, vNormal) * iTangent.w;
    vTexCoord4 = vec4(GetTexCoord(iTexCoord), bitangent.xy);
}

void PS()
{
    float colormix = fbm(vIPos.xyz*0.5);
    vec3 c = mix(vec3(1.0,1.0,0.0),vec3(1.0,0.0,0.0),bias(colormix,0.4));

    //gl_FragColor = diffColor * diffInput;
    #ifdef GLOW
        gl_FragColor =vec4(1.0,1.0,1.0,1.0);
    #else
        gl_FragColor = vec4(c,1.0);
    #endif
}