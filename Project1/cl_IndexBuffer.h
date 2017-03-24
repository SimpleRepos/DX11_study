#pragma once
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

///<summary>Encapsulation for an index buffer</summary>
class IndexBuffer {
public:
  ///<summary>Set this index buffer as active</summary>
  void set();

private:
  friend class GfxFactory;
  IndexBuffer(const std::vector<UINT>& indices, ID3D11Device* device, ID3D11DeviceContext* context);

  CComPtr<ID3D11DeviceContext> context;
  CComPtr<ID3D11Buffer> iBuffer;

};
