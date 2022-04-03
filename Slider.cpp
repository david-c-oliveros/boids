#include "Slider.h"


Slider::Slider()
    : Object({ 0.0f, 0.0f }) {}


Slider::Slider(olc::vf2d vPos, olc::vf2d vSize, olc::Pixel cColor)
    : Object(vPos), m_vSize(vSize), m_cColor(cColor) {}


Slider::~Slider() {}


void Slider::DrawSelf(olc::PixelGameEngine* pge)
{
    pge->FillRectDecal(m_vPos, m_vSize, m_cColor);
}
