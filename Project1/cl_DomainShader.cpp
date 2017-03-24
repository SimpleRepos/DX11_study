#include "cl_DomainShader.h"
#include "ns_Utility.h"

DomainShader::DomainShader(const std::string& filename, ID3D11Device* device, ID3D11DeviceContext* context) :
  context(context)
{
  if(!filename.empty()) {
    auto data = Utility::readFile(filename);

    HR(device->CreateDomainShader(data.data(), data.size(), nullptr, &dShader));

    D3D11_SAMPLER_DESC sDesc;
    sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sDesc.MipLODBias = 0;
    sDesc.MaxAnisotropy = 1;
    sDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    ZeroMemory(sDesc.BorderColor, sizeof(sDesc.BorderColor));
    sDesc.MinLOD = 0;
    sDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HR(device->CreateSamplerState(&sDesc, &sampler));
    context->DSSetSamplers(0, 1, &sampler.p);
  }
}

void DomainShader::set() {
  context->DSSetShader(dShader, 0, 0);
}
