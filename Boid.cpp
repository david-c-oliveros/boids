#include "Boid.h"


Boid::Boid(olc::vf2d vPos, olc::vf2d vVel)
    : Entity(vPos, vVel)
{
}


Boid::~Boid() {}


void Boid::DrawSelf(olc::TileTransformedView* tv)
{
    tv->FillCircle(m_vPos, 0.1f);
}
