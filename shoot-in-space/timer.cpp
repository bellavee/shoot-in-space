#include "timer.h"

timer::timer()
{
    this->m_seconds_per_count_ = 0;
    this->m_delta_time_ = -1.0;
    this->m_base_time_ = 0;
    this->m_paused_time_ = 0;
    this->m_prev_time_ = 0;
    this->m_curr_time_ = 0;
    this->m_stopped_ = false;

    LARGE_INTEGER counts_per_sec;
    QueryPerformanceFrequency(&counts_per_sec);
    m_seconds_per_count_ = 1.0 / static_cast<double>(counts_per_sec.QuadPart);
}

float timer::total_time() const
{
    if (m_stopped_)
    {
        return static_cast<float>(m_stop_time_ - m_paused_time_) - m_base_time_ * m_seconds_per_count_;
    }

    return static_cast<float>(m_curr_time_ - m_paused_time_) - m_base_time_ * m_seconds_per_count_;
}

float timer::delta_time() const
{
    return static_cast<float>(m_delta_time_);
}

void timer::reset()
{
    LARGE_INTEGER curr_time;
    QueryPerformanceCounter(&curr_time);

    m_base_time_ = curr_time.QuadPart;
    m_prev_time_ = curr_time.QuadPart; 
    m_stop_time_ = 0;
    m_stopped_ = false;
}

void timer::start()
{
    LARGE_INTEGER start_time;
    QueryPerformanceCounter(&start_time);

    if (m_stop_time_)
    {
        m_paused_time_ += (start_time.QuadPart - m_stop_time_);
        m_prev_time_ = start_time.QuadPart;
        m_stopped_ = false;
    }
}

void timer::stop()
{
    if (!m_stopped_)
    {
        LARGE_INTEGER curr_time;
        QueryPerformanceCounter(&curr_time);
        m_stop_time_ = curr_time.QuadPart;
        m_stopped_ = true;
    }
}

void timer::tick()
{
    if (m_stopped_)
    {
        m_delta_time_ = 0.0;
        return;
    }

    LARGE_INTEGER curr_time;
    QueryPerformanceCounter(&curr_time);
    m_curr_time_ = curr_time.QuadPart;
    m_delta_time_ = (m_curr_time_ - m_prev_time_) * m_seconds_per_count_;
    m_prev_time_ = m_curr_time_;

    if (m_delta_time_ < 0.0) m_delta_time_ = 0.0;
}


