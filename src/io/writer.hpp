#pragma once

#include <string>
#include "equation/grid.hpp"
#include "io/make_file.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/volumetric_field.hpp"
#include "io/make_datatype.hpp"
#include "H5Wrapper/include/h5_dataspace.hpp"
#include "H5Wrapper/include/h5_file.hpp"
#include "H5Wrapper/include/h5_dataset.hpp"
#include "H5Wrapper/include/h5_group.hpp"

struct Writer{


    Writer(std::string path) : m_file_path(path) {
        auto f = make_file(m_file_path);
        f.close();
    }





    template<size_t N>
    void write(const CartesianGrid<N>& grid){

        using namespace H5Wrapper;

        auto file = file_open();

        using element_type = Vector<N>;
        auto dt = H5DatatypeCreator<element_type>::create();
        auto dims = extent_to_array(grid.dimensions());

        std::string group_name = "grid";
        std::string dataset_name = "coords";
        auto location = H5Group::create(file, group_name);
        auto file_dataspace = H5Dataspace::create(dims);
        auto dataset = H5Dataset::create(location, dataset_name, dt, file_dataspace);
        auto data = points(grid);
        dataset.write(data.data());

        file.close();
        

    }






private:
    std::string m_file_path;


    auto file_open() const{
        using namespace H5Wrapper;
        return H5File::open(m_file_path, H5File::AccessFlag::READANDWRITE);
    }


};