#pragma once

#include "H5Wrapper/include/h5_file.hpp"

H5Wrapper::H5File make_file(const std::string& path)
{
    using namespace H5Wrapper;
    return H5File::create(path, H5File::CreationFlag::TRUNCATE);
}