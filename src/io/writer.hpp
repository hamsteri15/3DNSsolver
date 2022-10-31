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
#include <string>

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

template<class T>
struct Writeable{

    H5Wrapper::H5Dataspace memory_dataspace;
    H5Wrapper::H5Dataspace file_dataspace;
    H5Wrapper::H5Datatype  datatype;
    std::vector<T> data;
};

template<size_t N>
auto make_writeable(const Field<scalar>& f, extents<N> dims, extents<N> padding){
    using namespace H5Wrapper;
    auto                  dims_arr = extent_to_array(dims);
    auto                  padd_arr = extent_to_array(padding);
    std::array<size_t, N> total_dims{};

    // total = dims + 2*padding
    for (size_t i = 0; i < N; ++i) { total_dims[i] = dims_arr[i] + 2 * padd_arr[i]; }
    auto memory_parent_dataspace = H5Dataspace::create(total_dims);
    auto memory_dataspace = H5Hyperslab::select(memory_parent_dataspace, padd_arr, dims_arr);

    auto file_dataspace =  H5Dataspace::create(dims_arr);
    auto datatype = H5DatatypeCreator<scalar>::create();

    auto data = serialize(f);

    return Writeable<scalar>{memory_dataspace, file_dataspace, datatype, data};

}

template<size_t N, size_t L>
auto make_writeable(const Field<Vector<L>>& f, extents<N> dims, extents<N> padding){

    using namespace H5Wrapper;
    auto                  dims_arr = extent_to_array(dims);
    auto                  padd_arr = extent_to_array(padding);
    std::array<size_t, N> total_dims{};

    // total = dims + 2*padding
    for (size_t i = 0; i < N; ++i) { total_dims[i] = dims_arr[i] + 2 * padd_arr[i]; }

    std::array<size_t, N + 1> dims_arr2{};
    std::array<size_t, N + 1> padd_arr2{};
    std::array<size_t, N + 1> total_dims2{};
    for (size_t i = 0; i < N; ++i){
        dims_arr2[i] = dims_arr[i];
        padd_arr2[i] = padd_arr[i];
        total_dims2[i] = total_dims[i];

    }

    dims_arr2.back() = L;
    padd_arr2.back() = 0;
    total_dims2.back() = L;



    
    auto memory_parent_dataspace = H5Dataspace::create(total_dims2);
    auto memory_dataspace = H5Hyperslab::select(memory_parent_dataspace, padd_arr2, dims_arr2);

    auto file_dataspace =  H5Dataspace::create(dims_arr2);
    auto datatype = H5DatatypeCreator<scalar>::create();

    auto data = serialize(f);

    return Writeable<scalar>{memory_dataspace, file_dataspace, datatype, data};
    
}



struct Writer {

    Writer(std::string path)
        : m_file_path(path) {
        auto f = make_file(m_file_path);
        f.close();
    }

    template <size_t N> void write(const CartesianGrid<N>& grid) {

        std::string group_name = "grid";
        auto        data       = edges(grid);

        for (size_t i = 0; i < N; ++i) {
            std::array<size_t, 1> write_dims{data[i].size()};
            std::string           field_name = "c" + std::to_string(i);
            write<1, scalar>(data[i], write_dims, group_name, field_name);
        }

        if (N == 2){
            write("VXVY", group_name, "xdmf_geometry");
            write("2DRectMesh", group_name, "xdmf_topology");
        }
        
        if (N == 3){
            write("VXVYVZ", group_name, "xdmf_geometry");
            write("2DRectMesh", group_name, "xdmf_topology");
        }



    }

    template <size_t N, class ET>
    void write(const VolumetricField<ET, N>& field, std::string field_name, size_t checkpoint_i) {

        std::string group_name   = std::string("checkpoints/") + Constants::checkpoint_extension + std::to_string(checkpoint_i);
        std::string dataset_name = field_name;

        write<N, ET>(field, field.dimensions(), field.padding(), group_name, dataset_name);
    }

private:
    std::string m_file_path;

    
    void write(std::string str, std::string group_name, std::string field_name){
        using namespace H5Wrapper;


        std::vector<size_t> dims = {1};

        auto dt1 = H5DatatypeCreator<std::string>::create(str.size());
        auto memory_dataspace = H5Dataspace::create(dims);
        
        auto file = file_open();
        auto location = create_or_open(file, group_name);

        
        auto file_dataspace = H5Dataspace::create(dims);
        auto dataset = H5Dataset::create(location, field_name, dt1, file_dataspace);
        dataset.write(str.c_str());
    
        file.close();
        

    }
    

    template <size_t N, class ET>
    void write(const Field<ET>& field,
               extents<N>       dims,
               std::string      group_name,
               std::string      field_name) const {

        using namespace H5Wrapper;

        auto dt       = H5DatatypeCreator<ET>::create();
        auto dims_arr = extent_to_array(dims);

        auto file = file_open();

        auto location = create_or_open(file, group_name);
        
        auto file_dataspace = H5Dataspace::create(dims_arr);
        auto dataset        = H5Dataset::create(location, field_name, dt, file_dataspace);

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

        auto file = file_open();

        auto location = create_or_open(file, group_name);

        auto writeable = make_writeable<N>(field, dims, padding);
        auto dataset = H5Dataset::create(location, field_name, writeable.datatype, writeable.file_dataspace);

        dataset.write(writeable.data.data(), writeable.memory_dataspace);
        file.close();
    }

    template <size_t N> auto create_memory_dataspace(extents<N> dims, extents<N> padding) const {

        using namespace H5Wrapper;
        auto                  dims_arr = extent_to_array(dims);
        auto                  padd_arr = extent_to_array(padding);
        std::array<size_t, N> total_dims{};

        // total = dims + 2*padding
        for (size_t i = 0; i < N; ++i) { total_dims[i] = dims_arr[i] + 2 * padd_arr[i]; }
        auto memory_parent_dataspace = H5Dataspace::create(total_dims);
        auto memory_dataspace = H5Hyperslab::select(memory_parent_dataspace, padd_arr, dims_arr);
        return memory_dataspace;
    }

    H5Wrapper::H5File file_open() const {
        using namespace H5Wrapper;
        return H5File::open(m_file_path, H5File::AccessFlag::READANDWRITE);
    }

    H5Wrapper::H5Group create_or_open(H5Wrapper::H5File file, std::string group_name) const{
        using namespace H5Wrapper;
        H5Group location;
        if (H5Group::exists(file, group_name)) {
            location = H5Group::open(file, group_name);
        } else {
            location = H5Group::create(file, group_name);
        }
        return location;
    }


};