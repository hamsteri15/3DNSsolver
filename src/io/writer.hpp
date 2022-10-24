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

        std::string group_name = "grid";
        std::string dataset_name = "coords";

        auto dims = grid.dimensions();
        const auto data = points(grid);
        write<N, Vector<N>>(data, dims, group_name, dataset_name);

    }






private:
    std::string m_file_path;

    template<size_t N, class ET>
    void write(const Field<ET>& field, extents<N> dims, std::string group_name, std::string field_name) const{
        
        using namespace H5Wrapper;

        auto dt = H5DatatypeCreator<ET>::create();
        auto dims_arr = extent_to_array(dims);

        auto file = file_open();
        auto location = H5Group::create(file, group_name);
        auto file_dataspace = H5Dataspace::create(dims_arr);
        auto dataset = H5Dataset::create(location, field_name, dt, file_dataspace);

        dataset.write(field.data());

        file.close();
    }

    auto file_open() const{
        using namespace H5Wrapper;
        return H5File::open(m_file_path, H5File::AccessFlag::READANDWRITE);
    }



};