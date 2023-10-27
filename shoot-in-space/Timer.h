#pragma once

#include "pch.h"

class Timer
{
public:
    Timer();
    float TotalTime() const;
    float DeltaTime() const;

    void Reset();
    void Start();
    void Stop();
    void Tick();

private:
    double m_secondsPerCount;
    double m_deltaTime;

    int64_t  m_baseTime;
    int64_t m_pausedTime;
    int64_t m_stopTime;
    int64_t m_prevTime;
    int64_t m_currTime;

    bool m_stopped;
};
