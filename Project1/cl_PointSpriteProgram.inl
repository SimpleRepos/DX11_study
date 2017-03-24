#include "cl_PointSpriteProgram.h"
#include "cl_GfxFactory.h"

template<size_t SZ>
PointSpriteProgram<SZ>::PointSpriteProgram(GfxFactory& factory, const std::wstring& texture, const std::wstring& normals) :
  shaders(
    factory, 
    "../Debug/PointSpriteVShader.cso", D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, "", "",
    "../Debug/PointSpriteGShader.cso",
    RasterizerState::DEFAULT_DESC,
    "../Debug/PointSpritePShader.cso"
  ),
  cBuffer(factory.createConstantBuffer<cb>()),
  vBuffer(factory.createDynamicVertexBuffer<Vertex::PointSprite, SZ>()),
  tex(factory.createTexture(texture, D3D11_FILTER_MIN_MAG_MIP_POINT)),
  norms(factory.createTexture(normals, D3D11_FILTER_MIN_MAG_MIP_POINT))
{
  using namespace DirectX;
  cBuffer.object.useTex = true;
  vBuffer.data[0] = Vertex::PointSprite{ XMFLOAT4{ 0, 1, 0, 0 }, XMFLOAT4{ 0, 0, 1, 1 }, XMFLOAT2{ 1, 1 } };
}

template<size_t SZ>
void PointSpriteProgram<SZ>::set() {
  shaders.set();
  cBuffer.set(0);
  vBuffer.set(0);
  tex.set(0);
  norms.set(1);
}
