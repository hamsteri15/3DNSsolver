#pragma once

#include <string>
#include "equation/grid.hpp"
#include "io/make_file.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/volumetric_field.hpp"
#include "io/make_datatype.hpp"
#include "H5Wrapper/include/h5_dataspace.hpp"
#include "H5Wrapper/include/h5_dataspace_hyperslab.hpp"
#include "H5Wrapper/include/h5_file.hpp"
#include "H5Wrapper/include/h5_dataset.hpp"
#include "H5Wrapper/include/h5_group.hpp"

/*
template<class... Fields, class... Names>
struct Checkpoint{

    size_t n_dumps;
    std::string                  name;
    const double& time;
    std::tuple<const Fields&...> fields;
    std::tuple<const Names&....> field_names;

};
*/




struct Writer{


    Writer(std::string path) : m_file_path(path) {
        auto f = make_file(m_file_path);
        f.close();
    }


    template<size_t N>
    void write(const CartesianGrid<N>& grid){
        /*
        std::string group_name = "grid";
        std::string dataset_name = "XY";
        
        auto dims = grid.dimensions();
        const auto data = points(grid);
        write<N, Vector<N>>(data, dims, group_name, dataset_name);
        */
        
        std::string group_name = "grid";
        auto data = edges(grid);

        for (size_t i = 0; i < N; ++i){
            std::array<size_t, 1> write_dims{data[i].size()};
            std::string field_name = "c" + std::to_string(i);
            write<1, scalar>(data[i], write_dims, group_name, field_name);
        }

        /*
        const auto data = points(grid);
        auto write_dims = std::array<size_t, 1>{data.size()};
        write<1, Vector<N>>(data, write_dims, group_name, dataset_name);
        */
    }

    template <size_t N, class ET>
    void write(const VolumetricField<ET, N>& field, std::string field_name, size_t checkpoint_i) {

        std::string group_name   = std::string("data_") + std::to_string(checkpoint_i);
        std::string dataset_name = field_name;

        write<N, ET>(field, field.dimensions(), field.padding(), group_name, dataset_name);
    }

private:
    std::string m_file_path;





    template<size_t N, class ET>
    void write(const Field<ET>& field, extents<N> dims, std::string group_name, std::string field_name) const{
        
        using namespace H5Wrapper;

        auto dt = H5DatatypeCreator<ET>::create();
        auto dims_arr = extent_to_array(dims);

        auto file = file_open();
        
        H5Group location;

        if (H5Group::exists(file, group_name)){
            location = H5Group::open(file, group_name);
        }
        else{
            location = H5Group::create(file, group_name);
        }
        auto file_dataspace = H5Dataspace::create(dims_arr);
        auto dataset = H5Dataset::create(location, field_name, dt, file_dataspace);

        dataset.write(field.data());

        file.close();
    }

    template <size_t N, class ET>
    void write(const Field<ET>& field,
               extents<N>       dims,
               extents<N>       padding,
               std::string      group_name,
               std::string      field_name) const {

        using namespace H5Wrapper;

        auto                  dt       = H5DatatypeCreator<ET>::create();
        auto                  dims_arr = extent_to_array(dims);
        auto                  padd_arr = extent_to_array(padding);
        std::array<size_t, N> total_dims{};

        //total = dims + 2*padding
        for (size_t i = 0; i < N; ++i){
            total_dims[i] = dims_arr[i] + 2*padd_arr[i];
        }

        auto file = file_open();

        H5Group location;

        if (H5Group::exists(file, group_name)){
            location = H5Group::open(file, group_name);
        }
        else{
            location = H5Group::create(file, group_name);
        }

        auto memory_parent_dataspace = H5Dataspace::create(total_dims);
        auto memory_dataspace = H5Hyperslab::select(memory_parent_dataspace, padd_arr, dims_arr);

        auto file_dataspace = H5Dataspace::create(dims_arr);
        auto dataset        = H5Dataset::create(location, field_name, dt, file_dataspace);

        dataset.write(field.data(), memory_dataspace);
        location.close();
        file.close();
    }

    auto file_open() const{
        using namespace H5Wrapper;
        return H5File::open(m_file_path, H5File::AccessFlag::READANDWRITE);
    }



};