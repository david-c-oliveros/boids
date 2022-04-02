#pragma once

#include "Counter.h"
#include "Object.h"


class Button : public Object
{
    private:
        olc::vf2d m_vBaseSize;
        olc::vf2d m_vPressedSize;
        olc::vf2d m_vSize;
        olc::Pixel m_Color;
        Counter m_cEventCounter;

    public:
        std::string sText;
        Button();
        Button(olc::vf2d vPos, olc::vf2d vSize, std::string sText);
        ~Button();

        void Update();
        void DrawSelf(olc::PixelGameEngine* pge) override;
        bool ButtonHover(olc::vf2d vCursorPos);
        void Pressed();
};
