#pragma once
#include <string>
#include <atlbase.h>
#include <d3d11.h>

class DomainShader {
public:
  void set();

private:
  friend class GfxFactory;
  DomainShader(const std::string& filename, ID3D11Device* device, ID3D11DeviceContext* context);

  CComPtr<ID3D11DeviceContext> context;
  CComPtr<ID3D11DomainShader> dShader;
  CComPtr<ID3D11SamplerState> sampler;

};
