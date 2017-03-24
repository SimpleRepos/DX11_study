#include "cl_RasterizerState.h"
#include "ns_Utility.h"

const D3D11_RASTERIZER_DESC RasterizerState::DEFAULT_DESC = {
  D3D11_FILL_SOLID,
  D3D11_CULL_BACK,
  FALSE,
  0,
  0.0f,
  0.0f,
  TRUE,
  FALSE,
  FALSE,
  FALSE
};

void RasterizerState::set() {
  context->RSSetState(state);
}

RasterizerState::RasterizerState(ID3D11Device* device, ID3D11DeviceContext* context, const D3D11_RASTERIZER_DESC& desc) : 
  desc(desc),
  context(context)
{
  HR(device->CreateRasterizerState(&desc, &state.p));
}
