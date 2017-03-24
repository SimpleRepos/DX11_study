#pragma once
#include <chrono>

///<summary>High precision timer</summary>
class Timer {
public:
  ///<summary>Create Timer object and call its Timer::start()</summary>
  Timer();

  ///<summary>Reset the start time of the object to 'now'</summary>
  void start();

  ///<summary>Update the interal tick time, which represents the time since the last tick - see Timer::getTickDT()</summary>
  void tick();

  ///<summary>Return the number of seconds elapsed between the last two calls to this object's Timer::tick()</summary>
  double getTickDT() const;

  ///<summary>Return the number of seconds elapsed since the last call to this object's Timer::start()</summary>
  double sinceStart() const;

private:
  static std::chrono::time_point<std::chrono::high_resolution_clock> now();

  double dt;

  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> prev;

};

