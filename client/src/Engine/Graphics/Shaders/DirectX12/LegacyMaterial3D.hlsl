cbuffer LegacyConstants : register(b0) {
  float4 constants[13];
};
cbuffer MaterialState : register(b1) {
  float4 materialState0;
};
struct VSInput {
  float3 position : POSITION;
  float2 texCoord0 : TEXCOORD0;
  float4 color0 : COLOR0;
  float3 normal : NORMAL0;
  float2 texCoord1 : TEXCOORD1;
  float2 texCoord2 : TEXCOORD2;
  float2 texCoord3 : TEXCOORD3;
  float4 tangent : TANGENT0;
  float4 blendIndices : BLENDINDICES0;
  float4 blendWeights : BLENDWEIGHT0;
  float4 color1 : COLOR1;
  float clipW : TEXCOORD13;
  float4 texCoordQ : TEXCOORD14;
};
struct VSOutput {
  float4 position : SV_POSITION;
  float3 texCoord0 : TEXCOORD0;
  float3 texCoord1 : TEXCOORD1;
  float3 texCoord2 : TEXCOORD2;
  float3 texCoord3 : TEXCOORD3;
  float4 color0 : COLOR0;
  float4 color1 : COLOR1;
};
VSOutput VSMain(VSInput input) {
  VSOutput output;
  output.position = float4(input.position, input.clipW);
  output.texCoord0 = float3(input.texCoord0, input.texCoordQ.x);
  output.texCoord1 = float3(input.texCoord1, input.texCoordQ.y);
  output.texCoord2 = float3(input.texCoord2, input.texCoordQ.z);
  output.texCoord3 = float3(input.texCoord3, input.texCoordQ.w);
  output.color0 = input.color0;
  output.color1 = input.color1;
  return output;
}
Texture2D<float4> texture0 : register(t0);
Texture2D<float4> texture1 : register(t1);
Texture2D<float4> texture2 : register(t2);
Texture2D<float4> texture3 : register(t3);
SamplerState sourceSampler : register(s0);
float2 ResolveTexCoord(float3 coordinate) {
  float q = abs(coordinate.z) > 0.000001f ? coordinate.z : 1.0f;
  return coordinate.xy / q;
}
float2 ClampTexCoordToCenters(Texture2D<float4> textureValue,
  float2 coordinate) {
  uint width;
  uint height;
  textureValue.GetDimensions(width, height);
  float2 halfTexel = 0.5f / float2(width, height);
  return clamp(coordinate, halfTexel, 1.0f - halfTexel);
}
float4 ResolveFixedArgument(int encoded, float4 textureValue,
  float4 diffuse, float4 current, float4 temporary,
  float4 textureFactor, float4 stageConstant) {
  int selector = encoded & 15;
  float4 value = selector == 0 ? diffuse
    : selector == 1 ? current
    : selector == 2 ? textureValue
    : selector == 3 ? textureFactor
    : selector == 4 ? float4(0,0,0,0)
    : selector == 5 ? temporary
    : stageConstant;
  if ((encoded & 16) != 0) value = 1.0f - value;
  if ((encoded & 32) != 0) value = value.aaaa;
  return value;
}
float4 ApplyFixedOperation(int operation, float4 argument1,
  float4 argument2, float4 diffuse, float4 current,
  float4 textureValue, float4 textureFactor) {
  if (operation == 2) return argument1;
  if (operation == 3) return argument2;
  if (operation == 4) return argument1 * argument2;
  if (operation == 5) return argument1 * argument2 * 2.0f;
  if (operation == 6) return argument1 * argument2 * 4.0f;
  if (operation == 7) return argument1 + argument2;
  if (operation == 8) return argument1 + argument2 - 0.5f;
  if (operation == 9)
    return (argument1 + argument2 - 0.5f) * 2.0f;
  if (operation == 10) return argument1 - argument2;
  if (operation == 11)
    return argument1 + argument2 - argument1 * argument2;
  if (operation == 12)
    return lerp(argument2, argument1, diffuse.a);
  if (operation == 13)
    return lerp(argument2, argument1, textureValue.a);
  if (operation == 14)
    return lerp(argument2, argument1, textureFactor.a);
  if (operation == 15)
    return argument1 + argument2 * (1.0f - textureValue.a);
  if (operation == 16)
    return lerp(argument2, argument1, current.a);
  if (operation == 18)
    return argument1 + argument2 * argument1.a;
  if (operation == 19)
    return argument1 * argument2 + argument1.a;
  if (operation == 20)
    return argument1 + argument2 * (1.0f - argument1.a);
  if (operation == 21)
    return argument1.a + argument2 * (1.0f - argument1);
  if (operation == 24) {
    float value = dot(argument1.rgb * 2.0f - 1.0f,
      argument2.rgb * 2.0f - 1.0f);
    return float4(value, value, value, value);
  }
  return current;
}
float4 PSMain(VSOutput input) : SV_TARGET {
  const int mode = (int)(materialState0.x + 0.5f);
  float4 s0 = texture0.Sample(sourceSampler, ResolveTexCoord(input.texCoord0));
  float4 s1 = texture1.Sample(sourceSampler, ResolveTexCoord(input.texCoord1));
  float4 s2 = texture2.Sample(sourceSampler, ResolveTexCoord(input.texCoord2));
  float4 s3 = texture3.Sample(sourceSampler, ResolveTexCoord(input.texCoord3));
  if (mode == 0) {
    int passes = (int)(materialState0.y + 0.5f);
    float4 current = input.color0;
    float4 temporary = current;
    [unroll] for (int stage = 0; stage < 4; ++stage) {
      if (stage >= passes) break;
      float4 textureValue = stage == 0 ? s0
        : stage == 1 ? s1 : stage == 2 ? s2 : s3;
      int baseConstant = stage * 3;
      int colorOperation =
        (int)(constants[baseConstant].x + 0.5f);
      if (colorOperation == 1) break;
      int colorArgument1 =
        (int)(constants[baseConstant].y + 0.5f);
      int colorArgument2 =
        (int)(constants[baseConstant].z + 0.5f);
      int alphaOperation =
        (int)(constants[baseConstant].w + 0.5f);
      int alphaArgument1 =
        (int)(constants[baseConstant + 1].x + 0.5f);
      int alphaArgument2 =
        (int)(constants[baseConstant + 1].y + 0.5f);
      int resultArgument =
        (int)(constants[baseConstant + 1].z + 0.5f);
      float4 stageConstant = constants[baseConstant + 2];
      if (colorOperation == 1) break;
      float4 color1 = ResolveFixedArgument(colorArgument1,
        textureValue, input.color0, current, temporary,
        constants[12], stageConstant);
      float4 color2 = ResolveFixedArgument(colorArgument2,
        textureValue, input.color0, current, temporary,
        constants[12], stageConstant);
      float4 result = ApplyFixedOperation(colorOperation,
        color1, color2, input.color0, current,
        textureValue, constants[12]);
      if (alphaOperation != 1) {
        float4 alpha1 = ResolveFixedArgument(alphaArgument1,
          textureValue, input.color0, current, temporary,
          constants[12], stageConstant);
        float4 alpha2 = ResolveFixedArgument(alphaArgument2,
          textureValue, input.color0, current, temporary,
          constants[12], stageConstant);
        result.a = ApplyFixedOperation(alphaOperation,
          alpha1, alpha2, input.color0, current,
          textureValue, constants[12]).a;
      }
      if ((resultArgument & 15) == 5) temporary = result;
      else current = result;
    }
    if (materialState0.z > 0.5f)
      clip(current.a - materialState0.w);
    return saturate(current);
  }
  if (materialState0.z > 0.5f)
    clip(s0.a - materialState0.w);
  if (mode == 1) {
    float4 c = s0 * constants[0];
    return float4(c.rgb * c.a * input.color0.rgb * 2.0f,
      1.0f - c.a);
  }
  if (mode == 2) {
    float4 terrain0 = texture0.Sample(sourceSampler,
      ResolveTexCoord(input.texCoord0));
    float4 mask0 = texture1.Sample(sourceSampler,
      ClampTexCoordToCenters(texture1,
        ResolveTexCoord(input.texCoord1)));
    float4 terrain1 = texture2.Sample(sourceSampler,
      ResolveTexCoord(input.texCoord2));
    float4 mask1 = texture3.Sample(sourceSampler,
      ClampTexCoordToCenters(texture3,
        ResolveTexCoord(input.texCoord3)));
    float baseAlpha = terrain0.a * mask0.a;
    float blendAlpha = terrain1.a * mask1.a;
    float residualAlpha = (1.0f - baseAlpha)
      * (1.0f - blendAlpha);
    if (materialState0.w < 0.0f) residualAlpha = 0.0f;
    return float4(lerp(terrain1.rgb, terrain0.rgb * baseAlpha,
      1.0f - blendAlpha), residualAlpha);
  }
  if (mode == 3)
    return lerp(s1, s0, constants[7]) * constants[6];
  if (mode == 4)
    return constants[0] * constants[1];
  if (mode == 5) {
    float4 lit = saturate(s0 * input.color0 * float4(2,2,2,1));
    float4 reflection = s2 * constants[7];
    return float4(lerp(lit.rgb, reflection.rgb,
      reflection.a), lit.a);
  }
  if (mode == 6)
    return s0 * constants[0] * s1 * constants[1]
      * input.color0 * 4.0f;
  if (mode == 7) {
    float4 mask = texture1.Sample(sourceSampler,
      ClampTexCoordToCenters(texture1,
        ResolveTexCoord(input.texCoord1)));
    float alpha = s0.a * mask.a;
    float residualAlpha = materialState0.w < 0.0f
      ? 0.0f : 1.0f - alpha;
    return float4(s0.rgb * alpha, residualAlpha);
  }
  if (mode == 8) {
    float4 c = s0 * constants[0];
    return float4(c.rgb * c.a * input.color0.rgb * 2.0f,
      constants[1].x);
  }
  if (mode == 9)
    return s0 * input.color0;
  if (mode == 10) {
    float a = s0.a + constants[2].a;
    return float4(constants[0].rgb,
      a >= 0.5f ? constants[0].a : constants[1].a);
  }
  if (mode == 11)
    return s0;
  if (mode == 12) {
    float light = saturate(dot(s0.rgb * 2.0f - 1.0f,
      input.color1.rgb * 2.0f - 1.0f));
    float3 lighting = saturate(light * input.color0.rgb
      + constants[0].rgb);
    return float4(saturate(s1.rgb * lighting * 2.0f),
      s1.a * input.color0.a);
  }
  if (mode == 13)
    return saturate(s0 * input.color0 * float4(2,2,2,1));
  if (mode == 14) {
    float4 detailed = s0 * constants[0] * s1
      * constants[1] * input.color0 * 4.0f;
    return detailed;
  }
  if (mode == 15) {
    float4 base = s0 * constants[0];
    float4 reflection = s2 * constants[7];
    float alpha = base.a * (1.0f - reflection.a);
    return float4(base.rgb * alpha * input.color0.rgb * 2.0f,
      1.0f - alpha);
  }
  if (mode == 16) {
    float4 base = s0 * constants[0];
    float4 detail = s1 * constants[7];
    float alpha = base.a * (1.0f - detail.a);
    return float4(base.rgb * alpha * input.color0.rgb * 2.0f,
      1.0f - alpha);
  }
  if (mode == 17) {
    float light = saturate(dot(s0.rgb * 2.0f - 1.0f,
      input.color1.rgb * 2.0f - 1.0f));
    float3 lighting = saturate(light * input.color0.rgb
      + constants[0].rgb);
    float4 base = float4(saturate(s1.rgb * lighting * 2.0f),
      s1.a * input.color0.a);
    float4 reflection = s2 * constants[7];
    return float4(lerp(base.rgb, reflection.rgb,
      reflection.a), base.a);
  }
  if (mode == 18) {
    float light = saturate(dot(s0.rgb * 2.0f - 1.0f,
      input.color1.rgb * 2.0f - 1.0f));
    float3 lighting = saturate(light * input.color0.rgb
      + constants[0].rgb);
    float4 reflection = s2 * constants[7];
    return float4(saturate(s1.rgb * lighting * 2.0f),
      s1.a * input.color0.a * (1.0f - reflection.a));
  }
  if (mode == 19) {
    float light = saturate(dot(s0.rgb * 2.0f - 1.0f,
      input.color1.rgb * 2.0f - 1.0f));
    float3 lighting = saturate(light * input.color0.rgb
      + constants[0].rgb);
    float specular = s0.a * input.color1.a;
    return float4(saturate(s1.rgb * lighting * 2.0f
      + specular), s1.a * input.color0.a);
  }
  if (mode == 26) {
    float2 bump = s0.rg * 2.0f - 1.0f;
    float2 reflectionUV = input.texCoord1.xy + float2(
      dot(bump, constants[4].xz),
      dot(bump, constants[4].yw));
    float3 reflection = texture1.Sample(
      sourceSampler, saturate(reflectionUV)).rgb;
    return float4(reflection, constants[3].a);
  }
  if (mode == 20) return float4(s0.rgb, 1.0f);
  if (mode == 21) return float4(s1.rgb, 1.0f);
  if (mode == 22) return float4(s2.rgb, 1.0f);
  if (mode == 23) return float4(s3.rgb, 1.0f);
  if (mode == 24) return float4(s0.a, s1.a, s2.a, 1.0f);
  if (mode == 25) return float4(s3.aaa, 1.0f);
  int passes = (int)(materialState0.y + 0.5f);
  float4 fixedColor = input.color0;
  if (passes > 0) fixedColor *= s0;
  if (passes > 1) fixedColor *= s1;
  if (passes > 2) fixedColor *= s2;
  if (passes > 3) fixedColor *= s3;
  return fixedColor;
}
