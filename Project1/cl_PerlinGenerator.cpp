#include "cl_PerlinGenerator.h"
#include <random>

namespace {
  float calcAlpha(float x) {
    //6x^5 - 15x^4 + 10x^3 = Perlin's smoothed interpolation curve
    return ((6*x - 15)*x + 10)*x*x*x;
  }

  float interpolate(float a, float b, float alpha) {
    return a + (alpha * (b - a));
  }

  float dot(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b) {
    //DirectXMath is a massive PITA
    return (a.x * b.x) + (a.y * b.y);
  }
}

PerlinGenerator::PerlinGenerator(unsigned seed, size_t divisions) : STRIDE(divisions+1), vectorGrid(STRIDE*STRIDE) {
  std::mt19937 rng(seed);
  std::uniform_real_distribution<float> fdist(-1.0f, 1.0f);

  for(auto& vec : vectorGrid) {
    vec.x = fdist(rng);
    vec.y = fdist(rng);
  }
}

float PerlinGenerator::sample(DirectX::XMFLOAT2 texcoord) const {
  using namespace DirectX;

  int cellX = int(floorf(texcoord.x));
  int cellY = int(floorf(texcoord.y));

  XMFLOAT2 fromTpLt = texcoord;
  fromTpLt.x -= (float)cellX;
  fromTpLt.y -= (float)cellY;

  XMFLOAT2 fromBtRt = fromTpLt;
  fromBtRt.x--;
  fromBtRt.y--;
  
  float dots[4] = {
    dot(getCorner(cellX,   cellY  ), fromTpLt),
    dot(getCorner(cellX+1, cellY  ), XMFLOAT2(fromBtRt.x, fromTpLt.y)),
    dot(getCorner(cellX,   cellY+1), XMFLOAT2(fromTpLt.x, fromBtRt.y)),
    dot(getCorner(cellX+1, cellY+1), fromBtRt)
  };

  float xa = calcAlpha(fromTpLt.x);
  float topHorz = interpolate(dots[0], dots[1], xa);
  float botHorz = interpolate(dots[2], dots[3], xa);

  float ya = calcAlpha(fromTpLt.y);

  float value = interpolate(topHorz, botHorz, ya); //value in range -1...1
  value += 1;
  value *= 0.5f;

  return value;
}
