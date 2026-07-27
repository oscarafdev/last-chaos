cbuffer BloomState : register(b1) {
  float2 texelSize;
  float2 direction;
  float intensity;
  float threshold;
  float padding;
};
struct VSInput {
  float3 position : POSITION;
  float2 texCoord : TEXCOORD0;
};
struct VSOutput {
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
};
VSOutput VSMain(VSInput input) {
  VSOutput output;
  output.position = float4(input.position, 1.0f);
  output.texCoord = input.texCoord;
  return output;
}
Texture2D<float4> sourceTexture : register(t0);
SamplerState sourceSampler : register(s0);
float4 PSDownsample(VSOutput input) : SV_TARGET {
  float3 color = sourceTexture.Sample(
    sourceSampler, input.texCoord).rgb;
  float brightness = max(color.r, max(color.g, color.b));
  float contribution = saturate(
    (brightness - threshold) / max(1.0f - threshold, 0.001f));
  return float4(color * contribution, 1.0f);
}
float4 PSBlur(VSOutput input) : SV_TARGET {
  float2 stepUv = texelSize * direction;
  float3 color = sourceTexture.Sample(
    sourceSampler, input.texCoord).rgb * 0.227027f;
  color += sourceTexture.Sample(sourceSampler,
    input.texCoord + stepUv * 1.384615f).rgb * 0.316216f;
  color += sourceTexture.Sample(sourceSampler,
    input.texCoord - stepUv * 1.384615f).rgb * 0.316216f;
  color += sourceTexture.Sample(sourceSampler,
    input.texCoord + stepUv * 3.230769f).rgb * 0.070270f;
  color += sourceTexture.Sample(sourceSampler,
    input.texCoord - stepUv * 3.230769f).rgb * 0.070270f;
  return float4(color, 1.0f);
}
float4 PSComposite(VSOutput input) : SV_TARGET {
  float3 bloom = sourceTexture.Sample(
    sourceSampler, input.texCoord).rgb;
  return float4(bloom * intensity, 0.0f);
}
