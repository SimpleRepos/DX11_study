cbuffer cb {
  matrix viewProj;
  float3 tangent;
  bool useTex;
  float3 normal;
  float ambientRatio;
};

struct InVert {
  float4 bottomCenterPos : SV_POSITION;
  float4 srcRectLTRB : TEXCOORD0;
  float2 scaleXY : TEXCOORD1;
  float3 lightPos : TEXCOORD2;
};

struct OutVert {
  float4 posH : SV_POSITION;
  float2 tex  : TEXCOORD0;
  float3 posW : TEXCOORD1;
  float3 toLight : TEXCOORD2;
};

OutVert transform(OutVert vert) {
  vert.posH = mul(float4(vert.posW, 1), viewProj);
  vert.posH /= vert.posH.w;
  return vert;
}

[maxvertexcount(6)]
void main(point InVert input[1] : SV_POSITION, inout TriangleStream<OutVert> stream) {
  //calculate quad's rightward and topward edge offsets (from bottom center) in world space
  float3 worldRightEdgeOffset = (0.5 * input[0].scaleXY.x) * tangent;
  float3 worldTopEdgeOffset = float3(0, input[0].scaleXY.y, 0);

  //form the quad's vertices
  OutVert bl, tl, br, tr;

  //world space positions
  bl.posW = input[0].bottomCenterPos.xyz - worldRightEdgeOffset;
  br.posW = input[0].bottomCenterPos.xyz + worldRightEdgeOffset;
  tl.posW = bl.posW + worldTopEdgeOffset;
  tr.posW = br.posW + worldTopEdgeOffset;

  //transform to h space
  bl = transform(bl);
  tl = transform(tl);
  br = transform(br);
  tr = transform(tr);

  //tex coords from source rect
  bl.tex = float2(input[0].srcRectLTRB.x, input[0].srcRectLTRB.w);
  tl.tex = float2(input[0].srcRectLTRB.x, input[0].srcRectLTRB.y);
  br.tex = float2(input[0].srcRectLTRB.z, input[0].srcRectLTRB.w);
  tr.tex = float2(input[0].srcRectLTRB.z, input[0].srcRectLTRB.y);

  //light position
  bl.toLight = input[0].lightPos - bl.posW;
  tl.toLight = input[0].lightPos - tl.posW;
  br.toLight = input[0].lightPos - br.posW;
  tr.toLight = input[0].lightPos - tr.posW;

  //emit the triangles
  stream.Append(bl);
  stream.Append(tl);
  stream.Append(br);
  stream.RestartStrip();
  stream.Append(br);
  stream.Append(tl);
  stream.Append(tr);
  stream.RestartStrip();
}

