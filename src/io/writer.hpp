#ifndef writer_hh
#define writer_hh

#include <iomanip>  //std::setw
#include <iostream> //std::cout

#include "containers/array_4d.hpp"
#include "domain.hpp"
#include "io/output_info.hpp"

class Writer {

public:
    Writer()          = default;
    virtual ~Writer() = default;

    Writer(const char* outfile_path)
        : m_outfile_path(outfile_path) {}

    // Writer()

    virtual void write_mesh(const Grid& grid) const {}
    virtual void write_fields(const Domain& domain, size_t iter) const {}

    // for debugging
    void print_4d_array_slice(const Array4d& arr, int component, int slice);

protected:
    const char* m_outfile_path;

private:
    std::string m_type;
};

#endif
