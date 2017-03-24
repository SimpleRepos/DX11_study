#include "cl_HullShader.h"
#include "ns_Utility.h"

HullShader::HullShader(const std::string& filename, ID3D11Device* device, ID3D11DeviceContext* context) :
  context(context)
{
  if(!filename.empty()) {
    auto data = Utility::readFile(filename);

    HR(device->CreateHullShader(data.data(), data.size(), nullptr, &hShader));
  }
}

void HullShader::set() {
  context->HSSetShader(hShader, 0, 0);
}
