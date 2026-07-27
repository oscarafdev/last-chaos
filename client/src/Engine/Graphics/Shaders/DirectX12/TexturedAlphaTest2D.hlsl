struct VSInput {
  float2 position : POSITION;
  float2 texCoord : TEXCOORD0;
  float4 color : COLOR0;
};
struct VSOutput {
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;
  float4 color : COLOR0;
};
VSOutput VSMain(VSInput input) {
  VSOutput output;
  output.position = float4(input.position, 0.0f, 1.0f);
  output.texCoord = input.texCoord;
  output.color = input.color;
  return output;
}
Texture2D<float4> sourceTexture : register(t0);
SamplerState sourceSampler : register(s0);
float4 PSMain(VSOutput input) : SV_TARGET {
  float4 color = sourceTexture.Sample(
    sourceSampler, input.texCoord) * input.color;
  clip(color.a - (128.0f / 255.0f));
  return color;
}
