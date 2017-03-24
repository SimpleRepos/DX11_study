#pragma once

struct SharedState;

///<summary>Abstract base class for scenes - allows polymorphism in the top-level state machine</summary>
class Scene {
public:
  Scene(SharedState& shared);
  virtual ~Scene() = default;

  ///<summary>Update the simulation by SharedState::Timer::getTickDT() seconds</summary>
  virtual Scene* update() = 0;

  ///<summary>Render the scene</summary>
  virtual void draw() = 0;

protected:
  SharedState& shared;

};
