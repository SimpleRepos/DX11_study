#pragma once
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

///<summary>Encapsulation for a static (can not be changed) vertex buffer</summary>
class StaticVertexBuffer {
public:
  ///<summary>Assign this vertex buffer to a GPU register</summary>
  ///<param name="slotNum">Index of register to assign to</param>
  void set(UINT slotNum);

private:
  friend class GfxFactory;
  StaticVertexBuffer(const void* data, size_t numVertices, size_t stride, ID3D11Device* device, ID3D11DeviceContext* context);

  const size_t stride;
  CComPtr<ID3D11DeviceContext> context;
  CComPtr<ID3D11Buffer> vBuffer;

};
