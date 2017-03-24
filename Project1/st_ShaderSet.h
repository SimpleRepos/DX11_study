#pragma once
#include "cl_VertexShader.h"
#include "cl_HullShader.h"
#include "cl_DomainShader.h"
#include "cl_GeometryShader.h"
#include "cl_RasterizerState.h"
#include "cl_PixelShader.h"

struct ShaderSet {
  ShaderSet(
    GfxFactory& factory, 
    const std::string& vsFile, D3D11_PRIMITIVE_TOPOLOGY topology, 
    const std::string& hsFile, 
    const std::string& dsFile, 
    const std::string& gsFile, 
    const D3D11_RASTERIZER_DESC& desc,
    const std::string& psFile
  );
    
  void set();

  VertexShader vShader;
  HullShader hShader;
  DomainShader dShader;
  GeometryShader gShader;
  RasterizerState rState;
  PixelShader pShader;

};

