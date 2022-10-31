#pragma once

#include "H5Wrapper/include/h5_dataset.hpp"
#include "H5Wrapper/include/h5_dataspace.hpp"
#include "H5Wrapper/include/h5_dataspace_hyperslab.hpp"
#include "H5Wrapper/include/h5_file.hpp"
#include "H5Wrapper/include/h5_group.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/grid.hpp"
#include "equation/volumetric_field.hpp"
#include "io/make_datatype.hpp"
#include "io/make_file.hpp"
#include "io/constants.hpp"
#include "io/simple_xdmf.hpp"
#include <string>
#include <sstream>
#include <iterator>


struct Reader{

    Reader(std::string path)
        : m_file(H5Wrapper::H5File::open(path, H5Wrapper::H5File::AccessFlag::READ)) {}

    size_t checkpoint_count() const{
        return checkpoint_group().nlinks();
    }

    std::vector<H5Wrapper::H5Group> checkpoints() const {
        using namespace H5Wrapper;

        auto g = checkpoint_group();

        std::vector<H5Group> ret;

        for (size_t i = 0; i < checkpoint_count(); ++i) {

            std::string g_path = Constants::checkpoint_extension + std::to_string(i);
            ret.push_back(H5Group::open(g, g_path));
        }
        return ret;
    }

    


private:
    H5Wrapper::H5File m_file;
    

    H5Wrapper::H5Group checkpoint_group() const{
        return H5Wrapper::H5Group::open(m_file, std::string("checkpoints"));
    }

};

void write_grid(scalar time){
    (void) time;
    std::string grid_name = "\"Arbitrary_1\"";
    std::string grid_type = "\"Uniform\"";
    std::string time_value = std::string("\"") + std::to_string(time) + std::string("\"");
    std::stringstream ss;


    std::cout << std::endl;
    ss 
    << "<Grid Name=" << grid_name << " Grid Type=" << grid_type << ">\n"
    << "<Time Value=" << time_value << "/>\n";


    std::cout << ss.str() << std::endl;
    //std::cout << std::to_string(ss) << std::endl;

    /*
    std::string first = "<Grid Name=" + grid_name + std::string(" Grid Type=") + grid_type;
    std::string second = "<Time Value=" + std::to_string(time) + std::string()


    std::cout << first << std::endl;
    */
    



}

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

void write_xdmf(std::string path){

    Reader reader(path);

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

    gen.generate("output2.xdmf");

    (void) reader;

    write_grid(12.0);




}