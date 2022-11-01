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

