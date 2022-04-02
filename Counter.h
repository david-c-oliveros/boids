#pragma once


class Counter
{
    private:
        int m_iTicks;
        int m_iTickInterval;
        bool m_bRunning;

    public:
        Counter(int iTickInterval);
        ~Counter();

        void Start();
        void Reset();
        void Update();
        bool Check();
};
