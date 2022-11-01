#pragma once

#include "H5Wrapper/include/h5_dataset.hpp"
#include "H5Wrapper/include/h5_dataspace.hpp"
#include "H5Wrapper/include/h5_dataspace_hyperslab.hpp"
#include "H5Wrapper/include/h5_file.hpp"
#include "H5Wrapper/include/h5_group.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/grid.hpp"
#include "equation/volumetric_field.hpp"
#include "io/constants.hpp"
#include "io/make_datatype.hpp"
#include "io/simple_xdmf.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

struct XdmfGridInfo {

    std::string         topology_type;
    std::string         geometry_type;
    std::vector<size_t> vertex_dimensions;
};

struct Reader {

    Reader(std::string path)
        : m_file_path(path)
        , m_file(H5Wrapper::H5File::open(path, H5Wrapper::H5File::AccessFlag::READ)) {}

    size_t checkpoint_count() const { return checkpoint_group().nlinks(); }

    XdmfGridInfo read_xdmf_info() const {

        auto topo      = read_string(Constants::grid_xdmf_info_path, Constants::xdmf_topology_name);
        auto geom      = read_string(Constants::grid_xdmf_info_path, Constants::xdmf_geometry_name);
        auto vertex_dims = read_vertex_dimensions();
        return XdmfGridInfo{topo, geom, vertex_dims};
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

    std::string h5_file_path() const {return m_file_path;}

    /*
    std::vector<size_t> edge_count() const{

        using namespace H5Wrapper;
        //auto group = H5Group::open(Constants::grid_edge_path, )

    }
    */

    size_t read_spatial_dimensions() const {
        using namespace H5Wrapper;
        auto group = H5Group::open(m_file, Constants::grid_vertex_path);
        return group.nlinks();
    }

    std::vector<size_t> read_vertex_dimensions() const {
        using namespace H5Wrapper;
        size_t N = read_spatial_dimensions();

        std::vector<size_t> dims(N);

        for (size_t i = 0; i < N; ++i) {
            std::string field_name = Constants::vertex_extension + std::to_string(i);
            auto        group      = H5Group::open(m_file, Constants::grid_vertex_path);
            auto        dataset    = H5Dataset::open(group, field_name);
            auto        size       = dataset.get_dataspace().get_dimensions()[0];
            dims[i]                = size;
        }
        return dims;
    }

private:
    std::string       m_file_path;
    H5Wrapper::H5File m_file;

    template <class T>
    std::vector<T> read_array(std::string group_name, std::string field_name) const {
        using namespace H5Wrapper;
        auto           group     = H5Group::open(m_file, group_name);
        auto           dataset   = H5Dataset::open(group, field_name);
        auto           dataspace = dataset.get_dataspace();
        auto           size      = dataspace.get_dimensions()[0];
        std::vector<T> ret(size);
        dataset.read(ret.data());
        return ret;
    }

    std::string read_string(std::string group_name, std::string field_name) const {
        using namespace H5Wrapper;
        auto        group    = H5Group::open(m_file, group_name);
        auto        dataset  = H5Dataset::open(group, field_name);
        auto        datatype = dataset.get_datatype();
        auto        size     = datatype.get_size();
        std::string ret(size / sizeof(char), 'a');
        dataset.read(ret.data());
        return ret;
    }

    H5Wrapper::H5Group checkpoint_group() const {
        return H5Wrapper::H5Group::open(m_file, Constants::checkpoint_path);
    }
};