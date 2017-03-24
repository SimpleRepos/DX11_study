#include "cl_TerrainProgram.h"
#include <thread>

TerrainProgram::TerrainProgram(SharedState& shared) :
  shaders(
    shared.factory,
    "../Debug/passthruPosOnlyVShader.cso", D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
    "../Debug/tessHShader.cso", 
    "../Debug/tessDShader.cso", "",
    RasterizerState::DEFAULT_DESC,
    "../Debug/colorByYVal.cso"
  ),
  vBuffer(shared.factory.createStaticVertexBuffer(std::vector<DirectX::XMFLOAT3>{
    DirectX::XMFLOAT3( HALFWIDTH, 0, -HALFWIDTH),
    DirectX::XMFLOAT3(-HALFWIDTH, 0, -HALFWIDTH),
    DirectX::XMFLOAT3( HALFWIDTH, 0,  HALFWIDTH),
    DirectX::XMFLOAT3(-HALFWIDTH, 0,  HALFWIDTH),
  })),
  heightMapData(makeHMap()),
  heightMap(shared.factory.createTexture(DirectX::XMUINT2(HMAPWIDTH, HMAPWIDTH), heightMapData)),
  grass(shared.factory.createTexture(L"../Project1/grass.jpg")),
  dirt(shared.factory.createTexture(L"../Project1/dirt.png")),
  cBuffer(shared.factory.createConstantBuffer<cb>())
{
  cBuffer.object.heightScale = HEIGHT;
  cBuffer.object.lod = 64;
  cBuffer.object.ambientRatio = 0.25f;
  shared.win.addKeyFunc(VK_ADD,      [this](HWND, LPARAM) { cBuffer.object.lod++; cBuffer.object.lod = min(cBuffer.object.lod, 64); });
  shared.win.addKeyFunc(VK_SUBTRACT, [this](HWND, LPARAM) { cBuffer.object.lod--; cBuffer.object.lod = max(cBuffer.object.lod, 1); });
}

void TerrainProgram::draw(Graphics& gfx) {
  shaders.set();
  vBuffer.set(0);
  heightMap.set(0);
  grass.set(1);
  dirt.set(2);
  cBuffer.set(0);
  gfx.draw(4);
}

float TerrainProgram::sampleHeight(float x, float z) const {
  constexpr float WIDTH = HALFWIDTH * 2;

  float u = (x + HALFWIDTH) / WIDTH;
  float v = (z + HALFWIDTH) / WIDTH;
  u = 1.0f - u;

  //find texel and sample it
  size_t tx = (size_t)(u * (HMAPWIDTH - 1));
  size_t ty = (size_t)(v * (HMAPWIDTH - 1));
  size_t index = tx + (ty * HMAPWIDTH);
  uint8_t sample = heightMapData[index];
  return (float(sample) / 0xFF) * HEIGHT;;
}

namespace {
  void mapGen(std::vector<uint8_t> &texels, const PerlinGenerator& perlin, float scale, int width, size_t start, size_t end) {
    for(size_t y = start; y < end; y++) {
      for(int x = 0; x < width; x++)  {
        DirectX::XMFLOAT2 uv(x * scale, y * scale);
        texels[x + (y * width)] = (uint8_t)(perlin.sample(uv) * 0xFF);
      }
    }
  }
}

std::vector<uint8_t> TerrainProgram::makeHMap() {
  constexpr int WIDTH = HMAPWIDTH; //I say 'width', but this is a square, so it's also height

  std::vector<uint8_t> texels(WIDTH * WIDTH);

  PerlinGenerator perlin(std::random_device{}(), PERLIN_DIVS);

  constexpr float scale = (float)PERLIN_DIVS / WIDTH;

  size_t tCount = std::thread::hardware_concurrency();
  const size_t stride = WIDTH / tCount;

  std::vector<std::thread> threads;
  for(unsigned i = 0; i < tCount; i++) {
    size_t start = i * stride;
    size_t end = start + stride;
    if(i + 1 == tCount) { end = WIDTH; }
    threads.emplace_back(mapGen, std::ref(texels), std::cref(perlin), scale, WIDTH, start, end);
  }
  
  for(auto& t : threads) { t.join(); }

  return texels;
}
