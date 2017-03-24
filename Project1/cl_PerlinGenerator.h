#pragma once
#include <vector>
#include <DirectXMath.h>

class PerlinGenerator {
public:
  PerlinGenerator(unsigned seed, size_t divisions);
  float sample(DirectX::XMFLOAT2 texcoord) const;

private:
  DirectX::XMFLOAT2 getCorner(int x, int y) const { return vectorGrid[x + (y * STRIDE)]; }

  const size_t STRIDE;
  std::vector<DirectX::XMFLOAT2> vectorGrid;

};
