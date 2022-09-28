#pragma once

#include "equation/volumetric_field.hpp"
#include "io/make_datatype.hpp"
#include "H5Wrapper/include/h5_dataspace.hpp"
#include "H5Wrapper/include/h5_file.hpp"
#include "H5Wrapper/include/h5_dataset.hpp"

template<class ET, size_t N>
auto make_dataset(H5Wrapper::H5File& file, const VolumetricField<ET, N>& f){
    (void) f;
    (void) file;

    std::string dataset_name = "asd1/asd2/field";

    using namespace H5Wrapper;

    auto dims = extent_to_array(f.dims());

    

    auto datatype = H5DatatypeCreator<ET>::create();
    auto file_dataspace = H5Dataspace::create(dims);
    (void) datatype;
    (void) file_dataspace;

    return H5Dataset::create(file, dataset_name, datatype, file_dataspace);

}
