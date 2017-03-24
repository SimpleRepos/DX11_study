Texture2D heightMap;
SamplerState samp;

struct HS_CPOUT {
  float3 vPosition : WORLDPOS;
};

struct HS_CONST_OUT {
  float edges[4] : SV_TessFactor;
  float inside[2] : SV_InsideTessFactor;
};

cbuffer buf {
  matrix wvp;
  float3 lightPosA;
  float lod;
  float3 lightPosB;
  float heightScale;
  float3 lightPosC;
  float ambientRatio;
};

struct DOMAIN_OUT {
  float4 posH : SV_Position;
  float2 uv   : TEXCOORD0;
  float4 posW : TEXCOORD1;
};

[domain("quad")]
DOMAIN_OUT main(HS_CONST_OUT input, float2 uv : SV_DomainLocation, const OutputPatch<HS_CPOUT, 4> patch) {
  DOMAIN_OUT output;
  float3 topMid = lerp(patch[0].vPosition, patch[1].vPosition, uv.x);
  float3 botMid = lerp(patch[2].vPosition, patch[3].vPosition, uv.x);
  output.posW = float4(lerp(topMid, botMid, uv.y), 1);

  output.posW.y = heightMap.SampleLevel(samp, uv, 1.0f).r * heightScale;

  output.posH = mul(output.posW, wvp);
  output.uv = uv;

	return output;
}
