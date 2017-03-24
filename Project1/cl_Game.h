#pragma once
#include "st_SharedState.h"
#include <memory>

class Scene;

///<summary>Top-level game class</summary>
///<remarks>This class owns the SharedState object and is responsible for the state machine that runs the scenes.</remarks>
class Game {
public:
  ///<summary>Construct a game object</summary>
  ///<param name="winTitle">Title to use for game window</param>
  ///<param name="width">Width of user-area in pixels</param>
  ///<param name="height">Height of user-area in pixels</param>
  Game(const std::string& winTitle, long width, long height);

  ///<summary>Empty destructor - required for forward declaration with std::unique_ptr member</summary>
  ~Game();
  
  ///<summary>Generate the starting scene for the internal scene manager</summary>
  template<class START_SCENE>
  void initialize() { scene = std::make_unique<START_SCENE>(shared); }

  ///<summary>Execute the main control loop</summary>
  void run();

private:
  ///<summary>This object must not be copied or moved.</summary>
  Game(const Game&) = delete;

  ///<summary>This object must not be copied or moved.</summary>
  Game(Game&&) = delete;

  ///<summary>This object must not be copied or moved.</summary>
  void operator=(const Game&) = delete;

  ///<summary>This object must not be copied or moved.</summary>
  void operator=(Game&&) = delete;
  
  SharedState shared;
  std::unique_ptr<Scene> scene;

};

