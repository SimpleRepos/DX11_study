Texture2D tex : register(t0);
Texture2D nrmTex : register(t1);
SamplerState samp;

cbuffer cb {
  matrix viewProj;
  float3 tangent;
  bool useTex;
  float3 normal;
  float ambientRatio;
};

struct Vertex {
  float4 posH : SV_POSITION;
  float2 tex  : TEXCOORD0;
  float3 posW : TEXCOORD1;
  float3 toLight : TEXCOORD2;
};

float4 main(Vertex input) : SV_TARGET {
  float4 diffuse = float4(0.5, 0.5, 0.5, 1);
  if(useTex) { diffuse = tex.Sample(samp, input.tex); }
  float alpha = diffuse.a;
  clip(alpha - 0.5);
    
  float3 nrm = nrmTex.Sample(samp, input.tex).xyz;
  nrm = normalize((nrm * 2) - 1);
  nrm = (nrm.x * tangent) + float3(0, nrm.y, 0) + (nrm.z * normal);

  float lightDist = length(input.toLight);
  float3 toLight = normalize(input.toLight);

  float4 ambient = diffuse * ambientRatio;
  float4 lit = (1.0 - ambientRatio) * max(dot(toLight, nrm), 0);
  diffuse *= ambient + lit;
  diffuse.a = alpha;

  return diffuse * (ambient + lit);
}
