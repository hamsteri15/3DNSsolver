#pragma once

#include "io/reader.hpp"
#include "io/simple_xdmf.hpp"

void write_xdmf_checkpoint_attributes(const Reader& reader, SimpleXdmf& gen, size_t checkpoint_i){
    
    auto chk = reader.read_xdmf_checkpoint(checkpoint_i);

    auto n_fields = chk.field_names.size();

    std::string checkpoint_path 
        = reader.h5_file_path() 
        + std::string(":")
        + Constants::checkpoint_path
        + Constants::checkpoint_extension
        + std::to_string(checkpoint_i)
        + std::string("/");

    for (size_t i = 0; i < n_fields; ++i){

        gen.beginAttribute(chk.field_names.at(i), chk.field_types.at(i));
        gen.setCenter("Cell");

            gen.beginDataItem();
            gen.setNumberType("Float");
            gen.setPrecision("4");
            gen.setFormat("HDF");
            gen.setDimensions(chk.field_dims.at(i));

            gen.addItem(checkpoint_path + chk.field_names.at(i));
            //gen.setDimensions()
            gen.endDataItem();


        gen.endAttribute();

    }

}

void write_xdmf_checkpoint(const Reader& reader, SimpleXdmf& gen, size_t checkpoint_i) {

    auto info = reader.read_xdmf_info();
    auto dims = info.vertex_dimensions;

    gen.begin2DStructuredGrid("Main Topology", info.topology_type, dims[1], dims[0]);

    gen.beginTime();
    gen.setValue(std::to_string(checkpoint_i)); //!
    gen.endTime();

    gen.beginGeometry("Main Geometry", info.geometry_type);

    std::string edge_path 
        = reader.h5_file_path() 
        + std::string(":") 
        + Constants::grid_vertex_path;

    for (int i = dims.size() ; i-- > 0 ; ){

        std::string name = Constants::vertex_extension + std::to_string(i);
        gen.beginDataItem(name);
        gen.setDimensions(dims[i]);
        gen.setNumberType("Float");
        gen.setPrecision("4");
        gen.setFormat("HDF");
        gen.addItem(edge_path + name);
        gen.endDataItem();
    }

    gen.endGeometry();

    write_xdmf_checkpoint_attributes(reader, gen, checkpoint_i);

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

    for (size_t i = 0; i < reader.checkpoint_count(); ++i){
        write_xdmf_checkpoint(reader, gen, i);
    }

    gen.endGrid();
    gen.endDomain();

    gen.generate(o_path);

    (void)reader;
}
