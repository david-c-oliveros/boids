#pragma once

#include "Object.h"


class Slider : public Object
{
    private:
        olc::vf2d m_vSize;
        olc::Pixel m_cColor;

    public:
        Slider();
        Slider(olc::vf2d vPos, olc::vf2d vSize, olc::Pixel cColor = olc::BLUE);
        ~Slider();

        void DrawSelf(olc::PixelGameEngine* pge) override;
};
