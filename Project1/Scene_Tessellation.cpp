#include "Scene_Tessellation.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include "cl_PerlinGenerator.h"
#include <random>
#include <sstream>

using namespace DirectX;

Scene_Tessellation::Scene_Tessellation(SharedState& shared) :
  Scene(shared),
  debugFont(shared.factory.createFont(L"Courier New")),
  lampProg(shared.factory, L"../Project1/lamp.png", L"../Project1/lamp_n.png"),
  terrain(shared),
  player(cam, terrain, shared)
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  player.spriteProg.cBuffer.object.ambientRatio = 0.5f;

  //lamps
  lampProg.vBuffer.data[0].bottomCenterPos = XMFLOAT4( 5, 0,  0, 1);
  lampProg.vBuffer.data[1].bottomCenterPos = XMFLOAT4(-5, 0, -5, 1);
  lampProg.vBuffer.data[2].bottomCenterPos = XMFLOAT4( 5, 0,  5, 1);

  for(size_t i = 0; i < lampProg.vBuffer.data.size(); i++) {
    auto& vert = lampProg.vBuffer.data[i];
    float height = terrain.sampleHeight(vert.bottomCenterPos.x, vert.bottomCenterPos.z);
    vert.bottomCenterPos.y = height;
    vert.lightPos = XMFLOAT3(vert.bottomCenterPos.x, vert.bottomCenterPos.y, vert.bottomCenterPos.z);
    vert.lightPos.y += 1;
    vert.lightPos.z += 0.25f;
    vert.srcRectLTRB = XMFLOAT4(0, 0, 1.0f/3, 1);
    vert.scaleXY = XMFLOAT2(1, 1);
  }
  lampProg.vBuffer.update();

  lampProg.cBuffer.object.ambientRatio = 0;
  lampProg.cBuffer.object.useTex = true;

  terrain.cBuffer.object.lightPosA = lampProg.vBuffer.data[0].lightPos;
  terrain.cBuffer.object.lightPosB = lampProg.vBuffer.data[1].lightPos;
  terrain.cBuffer.object.lightPosC = lampProg.vBuffer.data[2].lightPos;

  //prep camera and rig
  camRig.length = 5;
  camRig.orbitRadians = 0;
  camRig.elevationRadians = -XM_PIDIV4 / 2;
  camRig.targetPos = DirectX::XMFLOAT3(0, 0, 0);
  cam.setAspectRatio((float)shared.gfx.VIEWPORT_DIMS.width / shared.gfx.VIEWPORT_DIMS.height);

  //bind camera orbit keys
  const float ORBIT_SPD = XM_PI / 32;
  shared.win.addKeyFunc('A', [ORBIT_SPD, this](HWND, LPARAM) {
    camRig.orbitRadians += ORBIT_SPD;
    if(camRig.orbitRadians > XM_2PI) { camRig.orbitRadians -= XM_2PI; }
  });
  shared.win.addKeyFunc('D', [ORBIT_SPD, this](HWND, LPARAM) { 
    camRig.orbitRadians -= ORBIT_SPD; 
    if(camRig.orbitRadians < 0) { camRig.orbitRadians += XM_2PI; }
  });

  //bind other camera keys
  constexpr float ELEV_SPD = 1.0f / 64;
  shared.win.addKeyFunc('W', [ELEV_SPD, this](HWND, LPARAM) { camRig.elevationRadians = max(camRig.elevationRadians - ELEV_SPD, -XM_PIDIV4); });
  shared.win.addKeyFunc('S', [ELEV_SPD, this](HWND, LPARAM) { camRig.elevationRadians = min(camRig.elevationRadians + ELEV_SPD, -XM_PIDIV4 / 4); });

  constexpr float ZOOM_SPD = 1.0f / 8;
  shared.win.addKeyFunc('Q', [ZOOM_SPD, this](HWND, LPARAM) { camRig.length = min(camRig.length + ZOOM_SPD, 10); });
  shared.win.addKeyFunc('E', [ZOOM_SPD, this](HWND, LPARAM) { camRig.length = max(camRig.length - ZOOM_SPD, 2); });
}

Scene* Scene_Tessellation::update() {
  XMFLOAT3 playerPos = player.getPos();

  camRig.targetPos = playerPos;
  cam.setEyePos(camRig);

  XMFLOAT4X4 xwvp = cam.getTransposedWVP(XMMatrixIdentity());
  XMFLOAT3 spriteTang = cam.getRightwardDirection();
  XMFLOAT3 spriteNorm = cam.getFlatBackward();

  auto& lampBuf = lampProg.cBuffer.object;
  lampBuf.viewProj = xwvp;
  lampBuf.normal = spriteNorm;
  lampBuf.tangent = spriteTang;
  lampProg.cBuffer.update();

  auto& terrainBuf = terrain.cBuffer.object;
  terrainBuf.wvp = xwvp;
  terrain.cBuffer.update();

  //this is lame, but I've spent enough time on this project now -.-
  XMFLOAT3 nearestLight;
  float nearDist = FLT_MAX;
  for(auto l : std::vector<XMFLOAT3*>{ &terrainBuf.lightPosA, &terrainBuf.lightPosB, &terrainBuf.lightPosC }) {
    float dist = abs(playerPos.x - l->x) + abs(playerPos.z - l->z);
    if(dist < nearDist) {
      nearDist = dist;
      nearestLight = *l;
    }
  }
  player.update(nearestLight, xwvp, spriteNorm, spriteTang);

  return this;
}

void Scene_Tessellation::draw() {
  shared.gfx.clear();
  terrain.draw(shared.gfx);
  player.draw(shared.gfx);
  lampProg.set();
  shared.gfx.draw(lampProg.vBuffer.SIZE);

  drawText();
}

void Scene_Tessellation::drawText() {
  std::wstringstream posVec;
  posVec << camRig.targetPos.x << ", " << camRig.targetPos.y << ", " << camRig.targetPos.z;
  size_t pvl = posVec.str().size();
  std::wstring blank(pvl, L' ');

  std::wstringstream words;
  words << "[+/-] Terrain LoD: \n";
  words << "[Dir] Position: [" << blank << "]\n";
  words << "[Q/E] Camera Arm Length: \n";
  words << "[A/D] Camera Orbit Angle(R): \n";
  words << "[W/S] Camera Elevation Angle(R): \n";
  debugFont.drawText(words.str(), 12, 2, 2, ColorF::RED);
  words.str(L"");
  words << "                   " << terrain.cBuffer.object.lod << "\n";
  words << "                 " << posVec.str() << " \n";
  words << "                         " << camRig.length << "\n";
  words << "                             " << camRig.orbitRadians << "\n";
  words << "                                 " << camRig.elevationRadians << "\n";
  debugFont.drawText(words.str(), 12, 2, 2, ColorF::GREEN);
}
