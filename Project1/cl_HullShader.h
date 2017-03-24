#pragma once
#include <string>
#include <atlbase.h>
#include <d3d11.h>

class HullShader {
public:
  void set();

private:
  friend class GfxFactory;
  HullShader(const std::string& filename, ID3D11Device* device, ID3D11DeviceContext* context);

  CComPtr<ID3D11DeviceContext> context;
  CComPtr<ID3D11HullShader> hShader;

};
