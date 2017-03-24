#include "cl_StaticVertexBuffer.h"
#include "ns_Utility.h"


void StaticVertexBuffer::set(UINT slotNum) {
  if(vBuffer == nullptr) {
    context->IASetVertexBuffers(slotNum, 0, 0, 0, 0);
    return;
  }

  constexpr UINT NUM_BUFFERS = 1;
  UINT strides[NUM_BUFFERS] = { stride };
  UINT offsets[NUM_BUFFERS] = { 0 };
  context->IASetVertexBuffers(slotNum, NUM_BUFFERS, &vBuffer.p, strides, offsets);
}


StaticVertexBuffer::StaticVertexBuffer(const void* data, size_t numVertices, size_t stride, ID3D11Device* device, ID3D11DeviceContext* context) :
  stride(stride),
  context(context)
{
  if(data == nullptr) { return; }

  D3D11_BUFFER_DESC vbDesc = { numVertices * stride, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
  D3D11_SUBRESOURCE_DATA vbData = { data, 0, 0 };

  HR(device->CreateBuffer(&vbDesc, &vbData, &vBuffer));
}

