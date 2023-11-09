#include "helper.h"


void Timer::InitSystemTime()
{
    LARGE_INTEGER frequency;
    memset(&frequency, 0, sizeof(LARGE_INTEGER));
    if (QueryPerformanceFrequency(&frequency) && frequency.QuadPart)
    {
        s_frequency = (float)frequency.QuadPart;
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        s_performTime = counter.QuadPart;
    }
}

float Timer::GetSystemTimeEx()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (float)(counter.QuadPart - s_performTime) / s_frequency;
}