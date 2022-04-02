#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Object
{
    protected:
        olc::vf2d m_vPos;

    public:
        Object(olc::vf2d vPos);
        ~Object();

        olc::vf2d GetPos();

        void SetPos(olc::vf2d vPos);

        virtual void DrawSelf(olc::PixelGameEngine* pge) {};
};
