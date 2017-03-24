struct InVert {
  float4 bottomCenterPos : POSITION0;
  float4 srcRectLTRB : TEXCOORD0;
  float2 scaleXY : TEXCOORD1;
  float3 lightPos : TEXCOORD2;
};

struct OutVert {
  float4 bottomCenterPos : SV_POSITION;
  float4 srcRectLTRB : TEXCOORD0;
  float2 scaleXY : TEXCOORD1;
  float3 lightPos : TEXCOORD2;
};

OutVert main(InVert input) {
  OutVert output = input;
  return output;
}
