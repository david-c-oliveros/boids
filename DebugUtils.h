#include "olcPixelGameEngine.h"

namespace debug
{
    void PrintVec(olc::vf2d vec, std::string name)
    {
        std::cout << name << ": " << "{ " << vec.x << ", " << vec.y << " }" << std::endl;
    }
}
