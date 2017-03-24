#include "cl_GeometryShader.h"
#include "ns_Utility.h"

GeometryShader::GeometryShader(const std::string& filename, ID3D11Device* device, ID3D11DeviceContext* context) :
  context(context)
{
  if(!filename.empty()) {
    auto data = Utility::readFile(filename);

    HR(device->CreateGeometryShader(data.data(), data.size(), nullptr, &gShader));
  }
}

void GeometryShader::set() {
  context->GSSetShader(gShader, 0, 0);
}
