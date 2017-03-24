#pragma once
#include "cl_Window.h"
#include "cl_Graphics.h"
#include "cl_Timer.h"
#include "cl_GfxFactory.h"

///<summary>Storage of centralized objects and access point for scenes to share persistent state</summary>
struct SharedState {
  SharedState(const std::string& windowTitle, Window::Dimensions dims) : 
    win(windowTitle, dims),
    gfx(win),
    factory(gfx.createFactory())
  {
    //nothing
  }

  Window win;
  Graphics gfx;
  GfxFactory factory;
  Timer timer;

};
