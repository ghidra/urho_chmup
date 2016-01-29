#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

uniform vec4 cGlow;

void PS()
{
  #ifdef GLOW
    gl_FragColor = vec4(1.0);
  #else
    gl_FragColor = vec4(0.0,0.0,0.0,1.0);
  #endif
}
