#include "RandUtils.h"
#include <random>

int generateInt(int min, int max)
{
  static thread_local std::random_device gen;
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}
