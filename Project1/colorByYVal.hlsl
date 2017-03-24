Texture2D height : register(t0);
Texture2D grassTex : register(t1);
Texture2D dirtTex : register(t2);
SamplerState samp;

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
  float2 uv : TEXCOORD0;
  float4 posW : TEXCOORD1;
};

float4 main(DOMAIN_OUT input) : SV_TARGET {
  float4 dirt  =  dirtTex.Sample(samp, input.uv * 8);
  float4 grassa = grassTex.Sample(samp, input.uv * 16);
  float4 grassb = grassTex.Sample(samp, input.uv * 8);
  float4 grass = (grassa + grassb) * 0.5;

  float y = height.Sample(samp, input.uv).r;

  //tune the blend value a bit with a polynomial curve to get more solid patches
  //of grass and dirt while retaining the smooth transitions between them
  float blend = saturate(((10*y - 8)*y + 10)*y*y*y);

  float4 diffuse = (grass * blend) + (dirt * (1 - blend));

  input.posW /= input.posW.w;
  float distA = distance(lightPosA, input.posW.xyz);
  float distB = distance(lightPosB, input.posW.xyz);
  float distC = distance(lightPosC, input.posW.xyz);
  float dist = min(min(distA, distB), distC);
  
  float4 ambient = diffuse * ambientRatio;
  diffuse *= 1.0 - ambientRatio;
  diffuse /= dist;

  diffuse += ambient;

  diffuse.a = 1;
  return diffuse;
}

