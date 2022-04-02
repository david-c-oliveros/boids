#include "Counter.h"


Counter::Counter(int iTickInterval) :
    m_iTickInterval(iTickInterval), m_bRunning(false), m_iTicks(0)
{
}


Counter::~Counter() {}


void Counter::Start()
{
    m_bRunning = true;
}


void Counter::Reset()
{
    m_bRunning = false;
    m_iTicks = 0;
}


void Counter::Update()
{
    if (m_bRunning)
    {
        m_iTicks++;
    }
}


bool Counter::Check()
{
    if (m_iTicks >= m_iTickInterval)
        return true;
    else
        return false;
}
