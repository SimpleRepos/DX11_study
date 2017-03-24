#include <Windows.h>
#include "cl_Game.h"
#include "Scene_Tessellation.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
  Game game("DX11 Project", 800, 600);
  game.initialize<Scene_Tessellation>();
  game.run();

}

