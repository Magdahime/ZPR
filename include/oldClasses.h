#pragma once
#include <random>

template <typename Numeric, typename Generator = std::mt19937>
Numeric random(Numeric from, Numeric to)
{
  thread_local static Generator gen(std::random_device{}());

  using dist_type = typename std::conditional<
      std::is_integral<Numeric>::value, std::uniform_int_distribution<Numeric>, std::uniform_real_distribution<Numeric>>::type;

  thread_local static dist_type dist;

  return dist(gen, typename dist_type::param_type{from, to});
}

struct obsoleteSpecimen
{
  float xPos;
  float yPos;
  float size;
  float age;
  float speed;
  float hdg;
  float r;
  float g;
  float b;
  obsoleteSpecimen()
  {
    obsoleteSpecimen(1000.0f, 1000.0f);
  }
  obsoleteSpecimen(float xMax, float yMax)
  {
    xPos = random<float>(0.0f, xMax);
    yPos = random<float>(0.0f, yMax);
    size = random<float>(20.0f, 50.0f);
    age = 0.0f;
    speed = random<float>(0.0f, sqrt(xMax * xMax + yMax * yMax) / 50.0f);
    hdg = random<float>(0.0f, 2.0f * 3.14159f);
    r = random<int>(100, 255);
    b = random<int>(100, 255);
    g = random<int>(100, 255);
  }
};

struct obsoleteNeuron
{
  float w1;
  float w2;
  float w3;
  float w4;
  float w5;
  float w6;
  obsoleteNeuron()
  {
    w1 = random<float>(-1.0f, 1.0f);
    w2 = random<float>(-1.0f, 1.0f);
    w3 = random<float>(-1.0f, 1.0f);
    w4 = random<float>(-1.0f, 1.0f);
    w5 = random<float>(-1.0f, 1.0f);
    w6 = random<float>(-1.0f, 1.0f);
    // std::cout<<"New neuron: "<<w1<<" "<<w2<<" "<<w3<<" "<<w4<<" "<<w5<<" "<<w6<<"\n";
  }
};