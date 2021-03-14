#ifndef output_info_hh
#define output_info_hh

#include <iostream> //std::cout
#include <string>

#include <cassert> //assert
#include <cstring>
#include <sstream>
#include <string>

class OutputInfo {

public:
    OutputInfo() {}

    OutputInfo(int output_interval, std::string type, std::string outfile_path);

    void update(double current_runtime, int current_iteration) {
        m_current_iteration = current_iteration;
        m_current_runtime   = current_runtime;
    }

    void print() const;

    int is_write_time() const;

    std::string get_file_path() const { return m_outfile_path; }

    std::string get_current_checkpoint() const;

    int grid_written() const { return m_grid_written == 1; }

    void wrote_grid() { m_grid_written = 1; }

    void increment_checkpoints() { m_number_of_checkpoints++; }

    double get_runtime() const { return m_current_runtime; }

private:
    int m_write_interval;
    int m_number_of_checkpoints;
    int m_grid_written;
    int m_current_iteration;

    double m_current_runtime;

    std::string m_type;
    std::string m_outfile_path;
};

#endif