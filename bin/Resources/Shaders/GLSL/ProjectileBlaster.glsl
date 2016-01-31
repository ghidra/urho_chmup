#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

#include "Functions.glsl"

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
    vec3 c = mix(vec3(0.0,0.0,1.0),vec3(0.0,1.0,0.0),bias(colormix,0.4));

    //gl_FragColor = diffColor * diffInput;
    #ifdef GLOW
        gl_FragColor =vec4(1.0,1.0,1.0,1.0);
    #else
        gl_FragColor = vec4(c,1.0);
    #endif
}