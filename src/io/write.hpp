#pragma once


#include "equation/euler_primitive_variables.hpp"


#include "equation/volumetric_field.hpp"
#include "io/make_datatype.hpp"
#include "H5Wrapper/include/h5_dataspace.hpp"
#include "H5Wrapper/include/h5_file.hpp"
#include "H5Wrapper/include/h5_dataset.hpp"

template<class ET, size_t N>
void write(H5Wrapper::H5File hf, const VolumetricField<ET, N>& f){

    std::string dataset_name = "asfd";

    using namespace H5Wrapper;

    auto dims = extent_to_array(full_extent(f));

    auto dt = H5DatatypeCreator<ET>::create();
    
    auto file_dataspace = H5Dataspace::create(dims);




    auto dataset = H5Dataset::create(hf, dataset_name, dt, file_dataspace);

    dataset.write(f.data());

}





