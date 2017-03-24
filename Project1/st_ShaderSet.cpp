#include "st_ShaderSet.h"
#include "cl_GfxFactory.h"

ShaderSet::ShaderSet(
  GfxFactory& factory,
  const std::string& vsFile, D3D11_PRIMITIVE_TOPOLOGY topology,
  const std::string& hsFile,
  const std::string& dsFile,
  const std::string& gsFile,
  const D3D11_RASTERIZER_DESC& desc,
  const std::string& psFile
) :
  vShader(factory.createVShader(vsFile, topology)),
  hShader(factory.createHShader(hsFile)),
  dShader(factory.createDShader(dsFile)),
  gShader(factory.createGShader(gsFile)),
  rState(factory.createRasterizerState(desc)),
  pShader(factory.createPShader(psFile))
{
  //nop
}

void ShaderSet::set() {
  vShader.set();
  hShader.set();
  dShader.set();
  gShader.set();
  rState.set();
  pShader.set();
}
