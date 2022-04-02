#include "Button.h"


Button::Button()
    : Object({ 0.0f, 0.0f }), m_cEventCounter(Counter(0)) {}


Button::Button(olc::vf2d vPos, olc::vf2d vSize, std::string sText)
    : Object(vPos), sText(sText), m_vBaseSize(vSize), m_vSize(vSize),
      m_cEventCounter(Counter(10)), m_Color(olc::BLUE)
{
    m_vPressedSize = { m_vBaseSize.x - 2.0f, m_vBaseSize.y - 2.0f };
}


Button::~Button() {}


void Button::Update()
{
    m_cEventCounter.Update();
    if (m_cEventCounter.Check())
    {
        m_cEventCounter.Reset();
        m_vSize = m_vBaseSize;
        m_vPos = { m_vPos.x - ((m_vBaseSize.x - m_vPressedSize.x) / 2),
                   m_vPos.y - ((m_vBaseSize.y - m_vPressedSize.y) / 2) };
    }
}


void Button::DrawSelf(olc::PixelGameEngine* pge)
{
    olc::vf2d s = { m_vSize.x + 1.0f, m_vSize.y + 1.0f };
    olc::vf2d p = { m_vPos.x - 1.0f, m_vPos.y - 1.0f };
    pge->FillRectDecal(p, s, olc::DARK_GREY);
    pge->FillRectDecal(m_vPos, m_vSize, m_Color);
    olc::vf2d tmp_pos = { m_vPos.x + 8, m_vPos.y + 5 };
    pge->DrawStringDecal(tmp_pos, sText, olc::BLACK);
}


bool Button::ButtonHover(olc::vf2d vCursorPos)
{
    bool bHover = (vCursorPos.x > m_vPos.x && vCursorPos.x < (m_vPos.x + m_vBaseSize.x)) &&
                  (vCursorPos.y > m_vPos.y && vCursorPos.y < (m_vPos.y + m_vBaseSize.y));
    if (bHover)
    {
        m_Color = olc::DARK_BLUE;
        return true;
    }
    m_Color = olc::BLUE;
    return false;
}


void Button::Pressed()
{
    // TODO - Fix text position and scale on press
    m_cEventCounter = Counter(5);
    m_cEventCounter.Start();
    m_vSize = m_vPressedSize;
    m_vPos = { m_vPos.x + ((m_vBaseSize.x - m_vPressedSize.x) / 2),
               m_vPos.y + ((m_vBaseSize.y - m_vPressedSize.y) / 2) };
}
