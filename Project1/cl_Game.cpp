#include "cl_Game.h"
#include "vcl_Scene.h"

Game::Game(const std::string& winTitle, long width, long height) :
  shared(winTitle, { width, height })
{
  //nothing
}

Game::~Game() {
  //this is required for Scene to be forward declared
}

void Game::run() {
  while(shared.win.update() && scene) {
    shared.timer.tick();

    Scene* next = scene->update();
    if(next != scene.get()) { scene.reset(next); }

    if(scene) {
      scene->draw();
      shared.gfx.present();
    }

  }

}

