#include "timer.hpp"

Timer::Timer()
    : m_start_time(0.0)
    , m_end_time(0.0)
    , m_elapsed_time(0.0) {}

double Timer::get_elapsed() const { return m_elapsed_time; }

void Timer::start() { m_start_time = get_current_wall_time(); }

void Timer::stop() {
    m_end_time = get_current_wall_time();
    m_elapsed_time += m_end_time - m_start_time;
}

void Timer::reset() { m_elapsed_time = 0.0; }

double Timer::get_current_wall_time() const {
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return static_cast<double>(tm.tv_sec) + static_cast<double>(tm.tv_usec) / 1E6;
}