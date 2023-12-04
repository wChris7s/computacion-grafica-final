#ifndef GLFW_OPERATIONS_H
#define GLFW_OPERATIONS_H

#include <random>
#include "ColorUtil.h"

Color getRandomRgbColor() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  float r = dis(gen);
  float g = dis(gen);
  float b = dis(gen);
  return {r, g, b};
}

float getRandom(float min, float max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

int getRandom(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(min, max);
  return dis(gen);
}

#endif //GLFW_OPERATIONS_H
