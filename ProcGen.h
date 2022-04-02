#include <math.h>
#include <chrono>
#include <iostream>
#include "olcPixelGameEngine.h"


namespace procgen
{
    static float normRand()
    {
        auto ti = std::chrono::system_clock::now().time_since_epoch().count();
        srand(ti);
        return ((float)rand() / (RAND_MAX));
    }

    static olc::vf2d GetRandomPointInCircle(float radius, olc::vf2d center)
    {
        float random = normRand();
        float r = radius * sqrt(random);
        float theta = random * 2 * M_PI;

        float cartX = center.x  + r * cos(theta);
        float cartY = center.y + r * sin(theta);

        olc::vf2d result = { cartX, cartY };
        return result;
    }
}
