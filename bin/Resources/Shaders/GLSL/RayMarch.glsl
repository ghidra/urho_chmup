#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

#include "Functions.glsl"

#define PI 3.14

//varying vec2 vTexCoord;
#ifdef COMPILEPS
uniform vec3 cShapeScale;
uniform float cRadius;
#endif
//varying mat4 vTransform;
varying vec3 vPos;
varying mat3 vRot;
//varying vec3 vEuler;
//varying mat3 vCameraRotation;

varying vec2 vScreenPos;
varying vec4 vScreenPos4;

//varying vec3 vNormal;
varying vec4 vWorldPos;

#ifdef PERPIXEL
    #ifdef SHADOW
        varying vec4 vShadowPos[NUMCASCADES];
    #endif
    #ifdef SPOTLIGHT
        varying vec4 vSpotPos;
    #endif
    #ifdef POINTLIGHT
        varying vec3 vCubeMaskVec;
    #endif
#else
    varying vec3 vVertexLight;
    #ifdef ENVCUBEMAP
        varying vec3 vReflectionVec;
    #endif
    #if defined(LIGHTMAP) || defined(AO)
        varying vec2 vTexCoord2;
    #endif
#endif

#ifdef COMPILEVS
//NOT USED
vec3 GetEuler(mat3 R){

    float x1 = -asin(R[0][2]);
    float x2 = PI - x1;

    float y1 = atan(R[1][2] / cos(x1), R[2][2] / cos(x1));
    float y2 = atan(R[1][2] / cos(x2), R[2][2] / cos(x2));

    float z1 = atan(R[0][1] / cos(x1), R[0][0] / cos(x1));
    float z2 = atan(R[0][1] / cos(x2), R[0][0] / cos(x2));
    return vec3(x1,y1,z1);
}
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    //vNormal = GetWorldNormal(modelMatrix);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));
    //vTransform = modelMatrix;
    vPos = (vec4(0.0,0.0,0.0,1.0)*modelMatrix).xyz;
    vRot = GetNormalMatrix(modelMatrix);
    //vEuler = GetEuler(mat3(modelMatrix));
    //vCameraRotation = cCameraRot;
    //vTexCoord = GetQuadTexCoord(gl_Position);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
    vScreenPos4 = GetScreenPos(gl_Position);

    #ifdef PERPIXEL // Per-pixel forward lighting
        vec4 projWorldPos = vec4(worldPos, 1.0);
        #ifdef SHADOW
            #ifdef VSM_SHADOW
                vTexCoord = vec3(GetTexCoord(iTexCoord), gl_Position.z / gl_Position.w * 0.5 + 0.5);
            #else
                vTexCoord = GetTexCoord(iTexCoord);
            #endif
            for (int i = 0; i < NUMCASCADES; i++)
                vShadowPos[i] = GetShadowPos(i, projWorldPos); // Shadow projection: transform from world space to shadow space
        #endif
        #ifdef SPOTLIGHT
            vSpotPos = projWorldPos * cLightMatrices[0];// Spotlight projection: transform from world space to projector texture coordinates
        #endif
        #ifdef POINTLIGHT
            vCubeMaskVec = (worldPos - cLightPos.xyz) * mat3(cLightMatrices[0][0].xyz, cLightMatrices[0][1].xyz, cLightMatrices[0][2].xyz);
        #endif
    #else // Ambient & per-vertex lighting
        vVertexLight = GetAmbient(GetZonePos(worldPos));
        #ifdef NUMVERTEXLIGHTS
            for (int i = 0; i < NUMVERTEXLIGHTS; ++i)
                vVertexLight += GetVertexLight(i, worldPos, vNormal) * cVertexLights[i * 3].rgb;
        #endif
        //vScreenPos = GetScreenPos(gl_Position);
        #ifdef ENVCUBEMAP
            vReflectionVec = worldPos - cCameraPos;
        #endif
    #endif

}

#ifdef COMPILEPS

vec3 mapP( vec3 p )
{
    //add gloabal time here to make new shapes
    p.xyz += 1.000*sin(  2.0*(p.yzx+vec3(0.0,cElapsedTimePS*0.5,0.0)) );//*grow.x;
    p.xyz += 0.500*sin(  4.0*(p.yzx+vec3(0.0,cElapsedTimePS*0.5,0.0)) );//*grow.y;
    p.xyz += 0.250*sin(  8.0*(p.yzx+vec3(0.0,cElapsedTimePS*0.5,0.0)) );//*grow.z;
    p.xyz += 0.050*sin( 16.0*(p.yzx+vec3(0.0,cElapsedTimePS*0.5,0.0)) );//*grow.w;
    return p;
}

float map( vec3 q )
{
    //q+=vec3(0.0,0.0,iGlobalTime*0.6);
    vec3 p = mapP( q );
    float d = length( p )-1.5;// - growf;
    return d * 0.05;
}

float intersect( in vec3 ro, in vec3 rd , in float start, in float end)
{
    float maxd = end;//start+7.0;

    float precis = 0.001;
    float h = (end-start)/128.0;//1.0;
    float t =  start;//1.0;
    for( int i=0; i<128; i++ )
    {
        //in here I do the clamping for symetry, and
        //scale for shaping
        if( (h<precis) || (t>maxd) ) break;
        vec3 mm = ro+rd*t;
        mm*=cShapeScale;//scale the shape
        mm=vec3(abs(mm.x),mm.y,mm.z);//mirror along x
        
        h = map( mm );
        t += h;
    }

    if( t>maxd ) t=-1.0;
    return t;
    //return fit(t,start,end,0.0,1.0);
}

vec3 calcNormal( in vec3 pos )
{
    vec3 eps = vec3(0.005,0.0,0.0);
    return normalize( vec3(
           map(pos+eps.xyy) - map(pos-eps.xyy),
           map(pos+eps.yxy) - map(pos-eps.yxy),
           map(pos+eps.yyx) - map(pos-eps.yyx) ) );
}

///---
////----
#endif

void PS()
{
    vec3 dir = normalize(vec3(vWorldPos)-cCameraPosPS);


    //vec3 vignette = vec3(1.0)*clamp(1.0-length(vec3(vScreenPos.r,vScreenPos.g,0.0)-0.5),0.0,1.0);
    //gl_FragColor = vec4(vig,1.0);//vScreenPos4;
    //gl_FragColor = vec4(dir,1.0);
    //vec3 pp = (vec3(0.0)*vTransform).xyz;
    vec3 difference = cCameraPosPS-vPos;
    vec3 offsetPosition = (difference)*transpose(vRot);
    vec3 offsetDirection = dir*transpose(vRot);
    float ldifference = length(difference);
    float inter = intersect(offsetPosition,offsetDirection,ldifference-cRadius,ldifference+cRadius);
    float io = clamp(inter*1000.0,0.0,1.0);

    if(io<=0.0)
        discard;

    vec3 pos = (offsetPosition+inter*offsetDirection)*cShapeScale;
    vec3 spos = vec3(abs(pos.x),pos.y,pos.z);//mirrored sample position
    vec3 normal = calcNormal(spos);
    normal*=vec3(clamp(pos.x*1000.0,-1.0,1.0),1.0,1.0);

    //gl_FragColor = vec4(normal*clamp(inter*1000.0,0.0,1.0),1.0);
    //gl_FragColor = vec4(vec3(inter),1.0);
     

    // Per-pixel forward lighting
    vec3 lightColor;
    vec3 lightDir;
    vec3 finalColor;

    //vec3 normal = normalize(vNormal);
   

    //finalColor = diff * lightColor * diffColor.rgb;
    //gl_FragColor = vec4(finalColor, diffColor.a);

    //gl_FragColor = vec4(vec3(diff),1.0);
    #ifdef SHADOW
        #ifdef VSM_SHADOW
            float depth = vTexCoord.z;
            gl_FragColor = vec4(depth, depth * depth, 1.0, 1.0);
        #else
            gl_FragColor = vec4(1.0);
        #endif
    #else
    
        float diff = GetDiffuse(normal, vWorldPos.xyz, lightDir);
        lightColor = cLightColor.rgb;
        finalColor = diff * lightColor * vec3(0.4,0.2,0.0);//diffColor.rgb;

        gl_FragColor = vec4(vec3(finalColor),1.0);
    #endif
    
}
