cbuffer LegacyConstants : register(b0) {
  float4 constants[13];
};
cbuffer LegacyPixelConstants : register(b1) {
  float4 pixelConstant0;
  float3 pixelConstant1;
};
struct VSInput {
  float3 position : POSITION;
  float2 texCoord : TEXCOORD0;
  float4 color : COLOR0;
  float3 normal : NORMAL0;
};
struct VSOutput {
  float4 position : SV_POSITION;
  float2 texCoord0 : TEXCOORD0;
  float2 texCoord1 : TEXCOORD1;
  float4 color : COLOR0;
};
VSOutput VSMain(VSInput input) {
  VSOutput output;
  float4 p = float4(input.position, 1.0f);
  output.position = float4(dot(p, constants[0]),
    dot(p, constants[1]), dot(p, constants[2]),
    dot(p, constants[3]));
  float3 n = normalize(input.normal);
  float lighting = clamp(dot(n, constants[4].xyz),
    constants[7].x, constants[7].y);
  output.color = float4(constants[5].xyz * lighting
    + constants[6].xyz, constants[7].y) * constants[7].y;
  output.texCoord0 = input.texCoord;
  output.texCoord1 = input.texCoord * constants[12].xy;
  return output;
}
Texture2D<float4> sourceTexture : register(t0);
Texture2D<float4> detailTexture : register(t1);
SamplerState sourceSampler : register(s0);
float4 PSMain(VSOutput input) : SV_TARGET {
  float4 base = sourceTexture.Sample(
    sourceSampler, input.texCoord0) * pixelConstant0;
  float4 detail = detailTexture.Sample(
    sourceSampler, input.texCoord1)
    * float4(pixelConstant1, 1.0f);
  return base * detail * input.color * 4.0f;
}
