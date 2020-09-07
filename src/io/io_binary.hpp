#ifndef io_binary_hh
#define io_binary_hh

#include <fstream> //std::ofstream

#include "writer.hpp"

class BinaryWriter : public Writer {

public:
    BinaryWriter()
        : Writer() {}
    BinaryWriter(const char* outfile_path)
        : Writer(outfile_path) {}

    virtual ~BinaryWriter() = default;

    virtual void write_mesh(const Grid& grid) const;
    virtual void write_fields(const Domain& domain, size_t iter) const;

    // virtual void write_mesh(Domain* d) const;
    // virtual void write_fields(Domain* d, int iter) const;
    virtual void write_mesh_gc(const Grid& grid, const Decomposition& dec) const;
    virtual void write_fields_gc(const Domain& domain, int iter) const;

private:
};

#endif
