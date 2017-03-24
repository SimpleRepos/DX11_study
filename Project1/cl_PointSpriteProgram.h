#pragma once
#include "st_ShaderSet.h"
#include "ns_Vertex.h"
#include "cl_Texture.h"
#include "tcl_ConstantBuffer.h"
#include "tcl_DynamicVertexBuffer.h"
#include <DirectXMath.h>

template<size_t SZ>
class PointSpriteProgram {
public:
  PointSpriteProgram(GfxFactory& factory, const std::wstring& texture, const std::wstring& normals);
  void set();

  ShaderSet shaders;

  struct cb {
    DirectX::XMFLOAT4X4 viewProj;
    DirectX::XMFLOAT3 tangent;
    bool useTex;
    DirectX::XMFLOAT3 normal;
    float ambientRatio;
  };

  ConstantBuffer<cb> cBuffer;
  DynamicVertexBuffer<Vertex::PointSprite, SZ> vBuffer;

private:
  Texture tex;
  Texture norms;

};

#include "cl_PointSpriteProgram.inl"
