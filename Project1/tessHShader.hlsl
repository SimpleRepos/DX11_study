cbuffer buf {
  matrix wvp;
  float3 lightPosA;
  float lod;
  float3 lightPosB;
  float heightScale;
  float3 lightPosC;
  float ambientRatio;
};

struct VS_OUT {
  float4 vPosition : SV_POSITION;
};

struct HS_CPOUT {
	float3 vPosition : WORLDPOS; 
};

struct HS_CONST_OUT {
	float edges[4]  : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

HS_CONST_OUT HSConstant(InputPatch<VS_OUT, 4> ip, uint PatchID : SV_PrimitiveID) {
  HS_CONST_OUT Output;

  Output.edges[0] = lod;
  Output.edges[1] = lod;
  Output.edges[2] = lod;
  Output.edges[3] = lod;
  Output.inside[0] = lod;
  Output.inside[1] = lod;

	return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HSConstant")]
HS_CPOUT main(InputPatch<VS_OUT, 4> ip, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID ) {
	HS_CPOUT Output;
	Output.vPosition = ip[i].vPosition.xyz;
	return Output;
}
