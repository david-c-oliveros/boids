#pragma once
#include "Object.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Entity : public Object
{
    protected:
        olc::vf2d m_vVel;

    public:
        Entity(olc::vf2d vPos, olc::vf2d vVel);
        ~Entity();

        olc::vf2d GetVel();

        void SetVel(olc::vf2d vVel);

        virtual void DrawSelf(olc::PixelGameEngine* pge, olc::TileTransformedView* tv) = 0;
};
