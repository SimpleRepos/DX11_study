#include "cl_Timer.h"

Timer::Timer() {
  startTime = prev = now();
}

void Timer::start() {
  startTime = now();
}

void Timer::tick() {
  auto cur = now();
  std::chrono::duration<double> seconds = cur - prev;
  prev = cur;
  dt = seconds.count();
}

double Timer::getTickDT() const {
  return dt;
}

double Timer::sinceStart() const {
  std::chrono::duration<double> seconds = now() - startTime;
  return seconds.count();
}

std::chrono::time_point<std::chrono::high_resolution_clock> Timer::now() {
  return std::chrono::high_resolution_clock::now();
}

