#pragma once

#include "olcPixelGameEngine.h"

class MathUtils
{
    public:
        static float ReMap(float x, float fromMin, float fromMax, float toMin, float toMax)
        {
            return (toMin + (x - fromMin) * (toMax - toMin) / (fromMax - fromMin));
        }


        static float DistanceBetweenPoints(olc::vf2d vecA, olc::vf2d vecB)
        {
            return sqrt((vecA.x - vecB.x) * (vecA.x - vecB.x) + (vecA.y - vecB.y) * (vecA.y - vecB.y));
        }
};
