#pragma once
#include <string>
#include <vector>
#include <atlbase.h>
#include <d3d11.h>
#include "cl_Font.h"
#include "tcl_ConstantBuffer.h"
#include "tcl_DynamicVertexBuffer.h"
#include <memory>
#include <DirectXMath.h>

class ImageLoader;
class Texture;
class IndexBuffer;
class VertexShader;
class GeometryShader;
class HullShader;
class DomainShader;
class PixelShader;
class StaticVertexBuffer;
class RasterizerState;

///<summary>Factory for graphics objects</summary>
class GfxFactory {
public:
  GfxFactory(ID3D11Device* device, ID3D11DeviceContext* context);
  GfxFactory(GfxFactory&&) = default;
  ~GfxFactory();

  ///<summary>Generate a Font object</summary>
  Font createFont(const std::wstring& fontFace);

  ///<summary>Generate an index buffer</summary>
  IndexBuffer createIndexBuffer(const std::vector<UINT>& indices);

  ///<summary>Generate a constant buffer</summary>
  template<class T>
  ConstantBuffer<T> createConstantBuffer() {
    return ConstantBuffer<T>(device.p, context.p);
  }

  ///<summary>Generate a static (contents can not be changed) vertex buffer</summary>
  template<class T>
  StaticVertexBuffer createStaticVertexBuffer(const std::vector<T>& vertices) {
    return StaticVertexBuffer(vertices.data(), vertices.size(), sizeof(T), device.p, context.p);
  }

  ///<summary>Generate a dynamic (contents can be changed) vertex buffer</summary>
  template<class T, size_t SZ>
  DynamicVertexBuffer<T, SZ> createDynamicVertexBuffer() {
    return DynamicVertexBuffer<T, SZ>(device.p, context.p);
  }

  ///<summary>Generate a vertex shader from a compiled shader object</summary>
  ///<param name="filename">Path to the .cso file to load</param>
  ///<param name="topology">Primitive topology expected by the shader</param>
  VertexShader createVShader(const std::string& filename, D3D11_PRIMITIVE_TOPOLOGY topology);

  ///<summary>Generate a geometry shader from a compiled shader object</summary>
  ///<param name="filename">Path to the .cso file to load - use an empty string to make this a null shader (for disabling the geometry shader)</param>
  GeometryShader createGShader(const std::string& filename);

  ///<summary>Generate a hull shader from a compiled shader object</summary>
  ///<param name="filename">Path to the .cso file to load - use an empty string to make this a null shader (for disabling the hull shader)</param>
  HullShader createHShader(const std::string& filename);

  ///<summary>Generate a domain shader from a compiled shader object</summary>
  ///<param name="filename">Path to the .cso file to load - use an empty string to make this a null shader (for disabling the domain shader)</param>
  DomainShader createDShader(const std::string& filename);

  ///<summary>Generate a pixel shader from a compiled shader object</summary>
  ///<param name="filename">Path to the .cso file to load</param>
  PixelShader createPShader(const std::string& filename);

  ///<summary>Generate a rasterizer state object</summary>
  ///<param name="desc">A description of the desired state</param>
  RasterizerState createRasterizerState(const D3D11_RASTERIZER_DESC& desc);

  ///<summary>Generate a texture from an image file</summary>
  ///<param name="filename">Path to the file to load</param>
  Texture createTexture(const std::wstring& filename, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

  ///<summary>Generate a texture from raw texel data</summary>
  ///<param name="dimensions">XY size of the texture to create</param>
  ///<param name="data">Raw texels</param>
  Texture createTexture(const DirectX::XMUINT2 dimensions, const std::vector<ColorF::uCol32>& data, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

  ///<summary>Generate a texture from raw texel data</summary>
  ///<param name="dimensions">XY size of the texture to create</param>
  ///<param name="data">Raw texels</param>
  Texture createTexture(const DirectX::XMUINT2 dimensions, const std::vector<uint8_t>& data, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

  std::pair<DirectX::XMUINT2, std::vector<ColorF::uCol32>> loadRawTextureData(const std::wstring& filename);

private:
  std::unique_ptr<ImageLoader> imageLoader;
  CComPtr<IFW1Factory> fontFactory;
  CComPtr<ID3D11Device> device;
  CComPtr<ID3D11DeviceContext> context;

};

