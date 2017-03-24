#pragma once
#include <atlbase.h>
#include <d3d11.h>

class RasterizerState {
public:
  const D3D11_RASTERIZER_DESC desc;

  static const D3D11_RASTERIZER_DESC DEFAULT_DESC;

  void set();

private:
  friend class GfxFactory;
  RasterizerState(ID3D11Device* device, ID3D11DeviceContext* context, const D3D11_RASTERIZER_DESC& desc = DEFAULT_DESC);

  CComPtr<ID3D11RasterizerState> state;
  CComPtr<ID3D11DeviceContext> context;

};
