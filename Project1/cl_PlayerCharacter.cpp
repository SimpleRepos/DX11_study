#include "cl_PlayerCharacter.h"
#include "cl_Camera.h"
#include "cl_Graphics.h"
#include <sstream>
#include "cl_Font.h"
#include "cl_TerrainProgram.h"
#include "st_SharedState.h"
#include "cl_Timer.h"

PlayerCharacter::PlayerCharacter(Camera& cam, TerrainProgram& terrain, SharedState& shared) :
  pos(0, 0, 0, 1),
  cam(cam),
  terrain(terrain),
  timer(shared.timer),
  moving(false),
  spriteProg(shared.factory, L"../Project1/spritesheet.png", L"../Project1/spritesheet_n.png")
{
  move(REFRESH);
  shared.win.addKeyFunc(VK_UP,    [this](HWND, LPARAM) { move(FORWARD); });
  shared.win.addKeyFunc(VK_DOWN,  [this](HWND, LPARAM) { move(BACKWARD); });
  shared.win.addKeyFunc(VK_RIGHT, [this](HWND, LPARAM) { move(RIGHT); });
  shared.win.addKeyFunc(VK_LEFT,  [this](HWND, LPARAM) { move(LEFT); });
  shared.win.addProcFunc(WM_KEYUP, [this](HWND,WPARAM,LPARAM) { moving = false; return 0; });
  spriteProg.cBuffer.object.useTex = true;
  spriteProg.cBuffer.object.ambientRatio = 0.5f;
}

void PlayerCharacter::update(DirectX::XMFLOAT3 lightPos, const DirectX::XMFLOAT4X4& viewProj, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangent) {
  using namespace DirectX;

  spriteProg.cBuffer.object.viewProj = viewProj;
  spriteProg.cBuffer.object.tangent = tangent;
  spriteProg.cBuffer.object.normal = normal;
  spriteProg.cBuffer.update();

  spriteProg.vBuffer.data[0].bottomCenterPos = pos;
  spriteProg.vBuffer.data[0].lightPos = lightPos;
  spriteProg.vBuffer.update();
}

void PlayerCharacter::draw(Graphics& gfx) {
  auto& vertex = spriteProg.vBuffer.data[0];
  vertex.bottomCenterPos = pos;

  constexpr float CELL_WIDTH  = 1.0f / (ANIM_FRAMES + 1);
  constexpr float CELL_HEIGHT = 1.0f / SPRITE_DIRECTIONS;
  constexpr int INNER_PERIOD = (2 * ANIM_FRAMES) - 2;

  int step = -1;
  if(moving) {
    step = (int)(timer.sinceStart() * ANIM_FPS) % INNER_PERIOD;
    if(step >= ANIM_FRAMES) { step = INNER_PERIOD - step; }
  }

  int dir;
  switch(facing) {
  case REFRESH:  dir = 0; break;
  case BACKWARD: dir = 0; break;
  case LEFT:     dir = 2; break;
  case RIGHT:    dir = 1; break;
  case FORWARD:  dir = 3; break;
  }

  vertex.srcRectLTRB.x = (step + 1) * CELL_WIDTH;
  vertex.srcRectLTRB.y = dir * CELL_HEIGHT;
  vertex.srcRectLTRB.z = vertex.srcRectLTRB.x + CELL_WIDTH;
  vertex.srcRectLTRB.w = vertex.srcRectLTRB.y + CELL_HEIGHT;

  spriteProg.vBuffer.update();

  spriteProg.set();
  gfx.draw(1);
}

DirectX::XMFLOAT3 PlayerCharacter::getPos() const {
  return { pos.x, pos.y, pos.z };
}

void PlayerCharacter::move(Direction dir) {
  using namespace DirectX;

  facing = dir;

  //camera orientation determines axes
  XMVECTOR rightward = XMLoadFloat3(&cam.getRightwardDirection()) * MOVE_SPEED;
  XMVECTOR forward   = XMLoadFloat3(&cam.getFlatForward())        * MOVE_SPEED;

  moving = true;

  //update position
  XMVECTOR posVec = XMLoadFloat4(&pos);
  switch(dir) {
  case FORWARD:  posVec += forward;   break;
  case BACKWARD: posVec -= forward;   break;
  case LEFT:     posVec -= rightward; break;
  case RIGHT:    posVec += rightward; break;
  case REFRESH:  moving = false;      break;
  }

  //stay inside the terrain area
  posVec.m128_f32[0] = min(terrain.HALFWIDTH, max(-terrain.HALFWIDTH, posVec.m128_f32[0]));
  posVec.m128_f32[2] = min(terrain.HALFWIDTH, max(-terrain.HALFWIDTH, posVec.m128_f32[2]));

  //set height
  posVec.m128_f32[1] = terrain.sampleHeight(posVec.m128_f32[0], posVec.m128_f32[2]);

  XMStoreFloat4(&pos, posVec);
}
