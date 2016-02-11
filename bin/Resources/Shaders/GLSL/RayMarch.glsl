#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

#define PI 3.14

//varying vec2 vTexCoord;
uniform vec3 cShapeScale;
//varying mat4 vTransform;
varying vec3 vPos;
varying mat3 vRot;
//varying vec3 vEuler;
//varying mat3 vCameraRotation;

varying vec2 vScreenPos;
varying vec4 vScreenPos4;

varying vec3 vNormal;
varying vec4 vWorldPos;
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif
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
    vNormal = GetWorldNormal(modelMatrix);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));
    //vTransform = modelMatrix;
    vPos = (vec4(0.0,0.0,0.0,1.0)*modelMatrix).xyz;
    vRot = GetNormalMatrix(modelMatrix);
    //vEuler = GetEuler(mat3(modelMatrix));
    //vCameraRotation = cCameraRot;
    //vTexCoord = GetQuadTexCoord(gl_Position);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
    vScreenPos4 = GetScreenPos(gl_Position);

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

float intersect( in vec3 ro, in vec3 rd )
{
    const float maxd = 7.0;

    float precis = 0.001;
    float h = 1.0;
    float t = 1.0;
    for( int i=0; i<256; i++ )
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
}

#endif

void PS()
{
    // Get material diffuse albedo

    
    // Get material specular albedo


    // Per-pixel forward lighting
    //vec3 lightColor;
    //vec3 lightDir;
    //vec3 finalColor;

    //float diff = GetDiffuse(normal, vWorldPos.xyz, lightDir);

    #ifdef SHADOW
        //diff *= GetShadow(vShadowPos, vWorldPos.w);
    #endif

    #if defined(SPOTLIGHT)
        //lightColor = vSpotPos.w > 0.0 ? texture2DProj(sLightSpotMap, vSpotPos).rgb * cLightColor.rgb : vec3(0.0, 0.0, 0.0);
    #elif defined(CUBEMASK)
        //lightColor = textureCube(sLightCubeMap, vCubeMaskVec).rgb * cLightColor.rgb;
    #else
        //lightColor = cLightColor.rgb;
    #endif

    #ifdef SPECULAR
        //float spec = GetSpecular(normal, cCameraPosPS - vWorldPos.xyz, lightDir, cMatSpecColor.a);
        //finalColor = diff * lightColor * (diffColor.rgb + spec * specColor * cLightColor.a);
    #else
        //finalColor = diff * lightColor * diffColor.rgb;
    #endif

    #ifdef AMBIENT
        //finalColor += cAmbientColor * diffColor.rgb;
        //finalColor += cMatEmissiveColor;
        //gl_FragColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);
    #else
        //gl_FragColor = vec4(GetLitFog(finalColor, fogFactor), diffColor.a);
    #endif
    vec3 dir = normalize(vec3(vWorldPos)-cCameraPosPS);


    vec3 vig = vec3(1.0)*clamp(1.0-length(vec3(vScreenPos.r,vScreenPos.g,0.0)-0.5),0.0,1.0);
    //gl_FragColor = vec4(vig,1.0);//vScreenPos4;
    //gl_FragColor = vec4(dir,1.0);
    //vec3 pp = (vec3(0.0)*vTransform).xyz;
    vec3 offsetPosition = cCameraPosPS-vPos;
    vec3 offsetDirection = dir*vRot;
    vec3 inter = vec3(intersect(offsetPosition,offsetDirection)*0.1);
    gl_FragColor = vec4(inter,1.0);
}
