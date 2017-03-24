#pragma once
#include <string>
#include <atlbase.h>
#include <d3d11.h>

///<summary>Class for encapsulating a D3D geometry shader - generate via Graphics::createGShader()</summary>
class GeometryShader {
public:
  ///<summary>Set this shader to be the active geometry shader</summary>
  void set();

private:
  friend class GfxFactory;
  GeometryShader(const std::string& filename, ID3D11Device* device, ID3D11DeviceContext* context);

  CComPtr<ID3D11DeviceContext> context;
  CComPtr<ID3D11GeometryShader> gShader;

};
