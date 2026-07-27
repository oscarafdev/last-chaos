struct VSInput {
  float3 position : POSITION;
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
  output.position = float4(input.position, 1.0f);
  output.texCoord = input.texCoord;
  output.color = input.color;
  return output;
}
Texture2D<float4> sourceTexture : register(t0);
SamplerState sourceSampler : register(s0);
float4 PSMain(VSOutput input) : SV_TARGET {
  return sourceTexture.Sample(sourceSampler, input.texCoord)
    * input.color;
}
