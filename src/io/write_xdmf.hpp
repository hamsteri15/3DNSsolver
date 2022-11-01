#pragma once

#include "io/simple_xdmf.hpp"
#include "io/reader.hpp"

void write_xdmf_checkpoint(SimpleXdmf& gen, size_t checkpoint_i){
    
    size_t nx = 51;
    size_t ny = 52;
    gen.begin2DStructuredGrid("Main Topology", "2DRectMesh", nx, ny);
    
    gen.beginTime();
    gen.setValue(std::to_string(checkpoint_i)); //!
    gen.endTime();   
    
    gen.beginGeometry("Main Geometry", "VXVY");
    
    gen.beginDataItem("ycoords");
    gen.setDimensions(ny-1);
    gen.setNumberType("Float");
    gen.setPrecision("4");
    gen.setFormat("HDF");
    gen.addItem("output.h5:/grid/c0");
    gen.endDataItem();
    
    gen.beginDataItem("xcoords");
    gen.setDimensions(nx-1);
    gen.setNumberType("Float");
    gen.setPrecision("4");
    gen.setFormat("HDF");
    gen.addItem("output.h5:/grid/c1");
    gen.endDataItem();
    

    //For all fields write attribute...



    gen.endGeometry();
    gen.end2DStructuredGrid();

    gen.setNewLineCodeLF();



}

void write_xdmf(std::string h5_file_path, std::string o_path){

    Reader reader(h5_file_path);

    SimpleXdmf gen;
    // Some settings
    gen.setNewLineCodeLF();
    gen.setIndentSpaceSize(2);


    gen.beginDomain("Domain");
    gen.beginGridCollection("Grid", "Collection", "Temporal");

    write_xdmf_checkpoint(gen, 0);
    write_xdmf_checkpoint(gen, 1);
    write_xdmf_checkpoint(gen, 2);

    gen.endGrid();    
    gen.endDomain();

    gen.generate(o_path);

    (void) reader;





}



