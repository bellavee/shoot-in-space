#include "Timer.h"

Timer::Timer()
{
    this->m_secondsPerCount = 0;
    this->m_deltaTime = -1.0;
    this->m_baseTime = 0;
    this->m_pausedTime = 0;
    this->m_prevTime = 0;
    this->m_currTime = 0;
    this->m_stopped = false;

    LARGE_INTEGER counts_per_sec;
    QueryPerformanceFrequency(&counts_per_sec);
    m_secondsPerCount = 1.0 / static_cast<double>(counts_per_sec.QuadPart);
}

float Timer::TotalTime() const
{
    if (m_stopped)
    {
        return static_cast<float>(m_stopTime - m_pausedTime) - m_baseTime * m_secondsPerCount;
    }

    return static_cast<float>(m_currTime - m_pausedTime) - m_baseTime * m_secondsPerCount;
}

float Timer::DeltaTime() const
{
    return static_cast<float>(m_deltaTime);
}

void Timer::Reset()
{
    LARGE_INTEGER currTime;
    QueryPerformanceCounter(&currTime);

    m_baseTime = currTime.QuadPart;
    m_prevTime = currTime.QuadPart; 
    m_stopTime = 0;
    m_stopped = false;
}

void Timer::Start()
{
    LARGE_INTEGER startTime;
    QueryPerformanceCounter(&startTime);

    if (m_stopTime)
    {
        m_pausedTime += (startTime.QuadPart - m_stopTime);
        m_prevTime = startTime.QuadPart;
        m_stopped = false;
    }
}

void Timer::Stop()
{
    if (!m_stopped)
    {
        LARGE_INTEGER currTime;
        QueryPerformanceCounter(&currTime);
        m_stopTime = currTime.QuadPart;
        m_stopped = true;
    }
}

void Timer::Tick()
{
    if (m_stopped)
    {
        m_deltaTime = 0.0;
        return;
    }

    LARGE_INTEGER currTime;
    QueryPerformanceCounter(&currTime);
    m_currTime = currTime.QuadPart;
    m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;
    m_prevTime = m_currTime;

    if (m_deltaTime < 0.0) m_deltaTime = 0.0;
}


