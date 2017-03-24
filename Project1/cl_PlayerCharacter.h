#pragma once
#include <DirectXMath.h>
#include <vector>
#include "st_ColorF.h"
#include "cl_PointSpriteProgram.h"

class Camera;
class Graphics;
class Font;
class TerrainProgram;
struct SharedState;
class Timer;

class PlayerCharacter {
public:
  PlayerCharacter(Camera& cam, TerrainProgram& terrain, SharedState& shared);
  void update(DirectX::XMFLOAT3 lightPos, const DirectX::XMFLOAT4X4& viewProj, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangent);
  void draw(Graphics& gfx);
  DirectX::XMFLOAT3 getPos() const;
  PointSpriteProgram<1> spriteProg;

private:
  static constexpr float MOVE_SPEED = 0.125f;
  static constexpr int ANIM_FPS = 6;
  static constexpr int ANIM_FRAMES = 3;
  static constexpr int SPRITE_DIRECTIONS = 4;

  enum Direction { LEFT, RIGHT, FORWARD, BACKWARD, REFRESH };
  void move(Direction dir);

  DirectX::XMFLOAT4 pos;
  Camera& cam;
  TerrainProgram& terrain;
  Timer& timer;
  bool moving;
  Direction facing;

};
