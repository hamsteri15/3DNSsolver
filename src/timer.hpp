#ifndef timer_hh
#define timer_hh

#include <cstddef>    //NULL
#include <sys/time.h> //gettimeofday

class Timer {
public:
    Timer();

    double get_elapsed() const;

    void start();
    void stop();

    void reset();

private:
    double m_start_time;
    double m_end_time;
    double m_elapsed_time;

    double get_current_wall_time() const;
};

#endif