#pragma once
#include <DirectXMath.h>

///<summary>Static storage for various vertex types</summary>
namespace Vertex {
  struct PointSprite {
    DirectX::XMFLOAT4 bottomCenterPos;
    DirectX::XMFLOAT4 srcRectLTRB;
    DirectX::XMFLOAT2 scaleXY;
    DirectX::XMFLOAT3 lightPos; 
  };

  struct Pos4Nrm4Tex2 {
    DirectX::XMFLOAT4 pos;
    DirectX::XMFLOAT4 normal;
    DirectX::XMFLOAT2 texcoord;
  };

  struct Pos3Tex2 {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
  };

}
