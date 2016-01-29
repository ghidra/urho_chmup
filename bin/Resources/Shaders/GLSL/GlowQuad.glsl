#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vTexCoord;
varying vec2 vScreenPos;
varying vec4 vScreenPos4;

#ifdef COMPILEPS
float bias(float t, float b)
{
    return (t / ((((1.0/b) - 2.0)*(1.0 - t))+1.0));
}
#endif

void VS()
{
  mat4 modelMatrix = iModelMatrix;
  vec3 worldPos = GetWorldPos(modelMatrix);
  gl_Position = GetClipPos(worldPos);
  vTexCoord = GetQuadTexCoord(gl_Position);
  vScreenPos = GetScreenPosPreDiv(gl_Position);
  vScreenPos4 = GetScreenPos(gl_Position);

}

void PS()
{

  vec4 normal = texture2D(sNormalMap,vScreenPos);
  vec4 diff = texture2D(sDiffMap,vScreenPos);

  float blursize=1.0;
  
  #ifdef MATTE
      gl_FragColor = normal*diff*bias(normal.g,0.1);
  #endif
  #ifdef BLURH
    vec3 rgb = texture2D(sDiffMap, vTexCoord + vec2(-2.0, 0.0) * blursize).rgb * 0.1;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(-1.0, 0.0) * blursize).rgb * 0.25;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 0.0) * blursize).rgb * 0.3;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(1.0, 0.0) * blursize).rgb * 0.25;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(2.0, 0.0) * blursize).rgb * 0.1;
    //
    //rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, -2.0) * blursize).rgb * 0.1;
    //rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, -1.0) * blursize).rgb * 0.25;
    //rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 0.0) * blursize).rgb * 0.3;
    //rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 1.0) * blursize).rgb * 0.25;
    //rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 2.0) * blursize).rgb * 0.1;
    gl_FragColor = vec4(rgb, 1.0);
  #endif
  #ifdef BLURV
    vec3 rgb = texture2D(sDiffMap, vTexCoord + vec2(0.0, -2.0) * blursize).rgb * 0.1;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, -1.0) * blursize).rgb * 0.25;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 0.0) * blursize).rgb * 0.3;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 1.0) * blursize).rgb * 0.25;
    rgb += texture2D(sDiffMap, vTexCoord + vec2(0.0, 2.0) * blursize).rgb * 0.1;
    gl_FragColor = vec4(rgb, 1.0);
  #endif
  #ifdef COMBINE
    gl_FragColor = vec4((diff+normal).rgb,1.0);
  #endif
  //gl_FragColor = color+diff;
  //;
}
