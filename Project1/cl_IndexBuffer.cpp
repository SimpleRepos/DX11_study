#include "cl_IndexBuffer.h"
#include "ns_Utility.h"

void IndexBuffer::set() {
  context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);
}

IndexBuffer::IndexBuffer(const std::vector<UINT>& indices, ID3D11Device* device, ID3D11DeviceContext* context) : 
  context(context) 
{
  if(indices.empty()) { return; }

  size_t size = indices.size() * sizeof(UINT);

  D3D11_BUFFER_DESC ibDesc = { size, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER,  0, 0, 0 };
  D3D11_SUBRESOURCE_DATA ibData = { indices.data(), 0, 0 };

  HR(device->CreateBuffer(&ibDesc, &ibData, &iBuffer));
}
