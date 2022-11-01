#pragma once

#include "io/reader.hpp"
#include "io/simple_xdmf.hpp"

void write_xdmf_checkpoint(const Reader& reader, SimpleXdmf& gen, size_t checkpoint_i) {

    auto info = reader.read_xdmf_info();
    auto dims = info.vertex_dimensions;

    gen.begin2DStructuredGrid("Main Topology", info.topology_type, dims[1], dims[0]);

    gen.beginTime();
    gen.setValue(std::to_string(checkpoint_i)); //!
    gen.endTime();

    gen.beginGeometry("Main Geometry", info.geometry_type);

    std::string edge_path = reader.h5_file_path() + std::string(":") + Constants::grid_vertex_path;

    for (int i = dims.size() - 1; i != -1; --i) {

        std::string name = Constants::vertex_extension + std::to_string(i);
        gen.beginDataItem(name);
        gen.setDimensions(dims[i]);
        gen.setNumberType("Float");
        gen.setPrecision("4");
        gen.setFormat("HDF");
        gen.addItem(edge_path + name);
        gen.endDataItem();
    }

    // For all fields write attribute...

    gen.endGeometry();
    gen.end2DStructuredGrid();

    gen.setNewLineCodeLF();
}

void write_xdmf(std::string h5_file_path, std::string o_path) {

    Reader reader(h5_file_path);

    SimpleXdmf gen;
    // Some settings
    gen.setNewLineCodeLF();
    gen.setIndentSpaceSize(2);

    gen.beginDomain("Domain");
    gen.beginGridCollection("Grid", "Collection", "Temporal");

    write_xdmf_checkpoint(reader, gen, 0);
    write_xdmf_checkpoint(reader, gen, 1);
    write_xdmf_checkpoint(reader, gen, 2);

    gen.endGrid();
    gen.endDomain();

    gen.generate(o_path);

    (void)reader;
}
