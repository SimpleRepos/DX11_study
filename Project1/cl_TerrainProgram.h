#pragma once
#include "st_SharedState.h"
#include "cl_PerlinGenerator.h"
#include <random>
#include <DirectXMath.h>
#include "st_ShaderSet.h"
#include "cl_Texture.h"
#include "cl_StaticVertexBuffer.h"

class TerrainProgram {
public:
  TerrainProgram(SharedState& shared);
  void draw(Graphics& gfx);
  float sampleHeight(float x, float z) const;

  struct cb {
    DirectX::XMFLOAT4X4 wvp;
    DirectX::XMFLOAT3 lightPosA;
    float lod;
    DirectX::XMFLOAT3 lightPosB;
    float heightScale;
    DirectX::XMFLOAT3 lightPosC;
    float ambientRatio;
  };

  ConstantBuffer<cb> cBuffer;

  static constexpr int PERLIN_DIVS = 6;
  static constexpr int HMAPWIDTH = 256;
  static constexpr float HALFWIDTH = 10;
  static constexpr float HEIGHT = 2;

private:
  static std::vector<uint8_t> makeHMap();
  const std::vector<uint8_t> heightMapData;

  ShaderSet shaders;
  StaticVertexBuffer vBuffer;

  Texture heightMap;
  Texture grass;
  Texture dirt;

};
