#pragma once
#include "vcl_Scene.h"
#include "cl_Camera.h"
#include "cl_Font.h"
#include "cl_PointSpriteProgram.h"
#include "cl_TerrainProgram.h"
#include "cl_PlayerCharacter.h"

class Scene_Tessellation : public Scene {
public:
  Scene_Tessellation(SharedState& shared);
  Scene* update() override;
  void draw() override;

private:
  void drawText();

  Camera cam;
  Camera::Arm camRig;

  TerrainProgram terrain;
  PointSpriteProgram<3> lampProg;
  Font debugFont;

  PlayerCharacter player;

};

