#pragma once

#include "pch.h"

class timer
{
public:
    timer();
    float total_time() const;
    float delta_time() const;

    void reset();
    void start();
    void stop();
    void tick();

private:
    double m_seconds_per_count_;
    double m_delta_time_;

    int64_t  m_base_time_;
    int64_t m_paused_time_;
    int64_t m_stop_time_;
    int64_t m_prev_time_;
    int64_t m_curr_time_;

    bool m_stopped_;
};
