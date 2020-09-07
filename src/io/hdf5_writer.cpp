#include "./hdf5_writer.hpp"

Hdf5Writer::Hdf5Writer(const char* outfile_path) : Writer(outfile_path){

    

}

void Hdf5Writer::write(OutputInfo& info, const Grid& grid, const Domain& domain, const Decomposition& dec, const Communicator& comm, const Params& p){

    if ( !info.grid_written() ){
        create_file( info.get_file_path() );
        create_group_to_file(info.get_file_path(), "mesh");
        std::vector<std::string> datasets = {"x_coords", "y_coords", "z_coords"};

        write_4d_array( grid.get_coords(), info.get_file_path(), "mesh", datasets, p, dec, comm);
        
        info.wrote_grid(); //set grid_written = True

    }


    

    if ( info.is_write_time() ){

        info.print();

       


        create_group_to_file(info.get_file_path(), info.get_current_checkpoint());
        write_time_info_to_group(info.get_file_path(), info.get_current_checkpoint(), info.get_runtime() );

        std::vector<std::string> datasets = {"rho", "p", "u", "v", "w"};

        write_4d_array( domain.get_NS_primitive_array(), info.get_file_path(), info.get_current_checkpoint(), datasets, p, dec, comm);
        
        

        info.increment_checkpoints();     




        

    }


}

void Hdf5Writer::create_file(std::string f_path){

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0){    
        hid_t file_id = H5Fcreate(f_path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        file_close(file_id);
    }


}







hid_t Hdf5Writer::get_parallel_file_access(const char* file_path, MPI_Comm comm) const{

	//open file parallel read and write properties
    hid_t acc_tpl1 = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(acc_tpl1, comm, MPI_INFO_NULL);				
	hid_t fid1 = H5Fopen(file_path, H5F_ACC_RDWR, acc_tpl1 );

    memory_close(acc_tpl1);
	return fid1;
	
}

hid_t Hdf5Writer::get_serial_file_access(const char* file_path) const{


    hid_t fid1 = H5Fopen(file_path, H5F_ACC_RDWR, H5P_DEFAULT );
    return fid1;

}


hid_t Hdf5Writer::create_3d_file_dataspace(const Params& p, const Decomposition& dec){

    //all arrays in the file are three-dimensional
    int rank = 3;

    hid_t err;

    hsize_t dimens_3d[3];
    hsize_t start_3d[3];
    hsize_t stride_3d[3];
    hsize_t count_3d[3];

    //total dimensions in the file (taken from the input params)
    dimens_3d[0] = p.nx;
    dimens_3d[1] = p.ny;
    dimens_3d[2] = p.nz;

    hid_t dataspace = H5Screate_simple(rank, dimens_3d, NULL);

    std::vector<int> start_indices = dec.get_subdomain_start_idx();
    //define where to write in the file
    start_3d[0] = start_indices[0];
    start_3d[1] = start_indices[1];
    start_3d[2] = start_indices[2];

    //always just ones
    stride_3d[0] = 1; 
    stride_3d[1] = 1;
    stride_3d[2] = 1;

    std::vector<int> local_dimensions = dec.get_subdomain_node_count(p);
    count_3d[0] = local_dimensions[0]; 
    count_3d[1] = local_dimensions[1];
    count_3d[2] = local_dimensions[2];

    err = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_3d, stride_3d, count_3d, NULL);
    assert(err != FAIL);

    return dataspace;

}


hid_t Hdf5Writer::create_3d_memory_dataspace(const Params& p, const Decomposition& dec){

    //all arrays in the file are three-dimensional
    int rank = 3;

    hid_t err;

    hsize_t dimens_3d[3];
    hsize_t start_3d[3];
    hsize_t stride_3d[3];
    hsize_t count_3d[3];

    std::vector<int> local_dimensions = dec.get_subdomain_node_count(p);
    dimens_3d[0] = local_dimensions[0];
    dimens_3d[1] = local_dimensions[1];
    dimens_3d[2] = local_dimensions[2];

    hid_t dataspace = H5Screate_simple(rank, dimens_3d, NULL);
    /*
    //NO NEED TO SLAB THE MEMORY DATASPACE SINCE THE CREATED SERIALIZED ARRAY IS ALREADY CONTIGUOUS
    //local dimensions
    start_3d[0] = 0;
    start_3d[1] = 0;
    start_3d[2] = 0;

    stride_3d[0] = 1;
    stride_3d[1] = 1;
    stride_3d[2] = 1;

    count_3d[0] = local_dimensions[0];
    count_3d[1] = local_dimensions[1];
    count_3d[2] = local_dimensions[2];

    
    err = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, start_3d, stride_3d, count_3d, NULL);
    assert(err != FAIL);
    */
    return dataspace;


}


void Hdf5Writer::create_group_to_file(std::string f_path, std::string group_name){

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0){    
        hid_t file_id = get_serial_file_access(f_path.c_str());
        hid_t group = H5Gcreate2(file_id, group_name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        group_close(group);
        file_close(file_id);
    }

}



void Hdf5Writer::write_time_info_to_group(std::string f_path, std::string group_name, double runtime){


    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0){   
        int rank = 1;
        hsize_t dims[1] = {1};


        hid_t file_id = get_serial_file_access(f_path.c_str());

        hid_t file_dataspace = H5Screate_simple(rank, dims, NULL);
        hid_t memory_dataspace = H5Screate_simple(rank, dims, NULL);



        std::string dataset_name = "/" + group_name + "/time";


        hid_t dataset = H5Dcreate(file_id, dataset_name.c_str(), H5T_NATIVE_DOUBLE, file_dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        hid_t err = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, memory_dataspace, file_dataspace, H5P_DEFAULT, &(runtime));
        if (err == FAIL) throw H5WriteError(dataset_name.c_str(), "asd");
        

        dataspace_close(memory_dataspace);
        dataspace_close(file_dataspace);
        dataset_close(dataset);
        file_close(file_id);
    }

}

void Hdf5Writer::write_4d_array(const Array4d& array, std::string file_path, std::string group_name, std::vector<std::string> dataset_names, 
                    const Params& p, const Decomposition& dec, const Communicator& comm )
{


    //serialized data
    double* serialized = new double[array.nx() * array.ny() * array.nz()];


    hid_t fid = get_parallel_file_access(file_path.c_str(), comm.get_communicator());
    hid_t file_dataspace = create_3d_file_dataspace(p, dec);
    hid_t memory_dataspace = create_3d_memory_dataspace(p, dec);

    
    for (size_t q = array.q_begin(); q < array.q_end(); q++){
        
        serialize_4d_array_at_index(array, serialized, q);
        
        //this path is relative to the input file (fid)
        std::string data_path = "/" + group_name + "/" + dataset_names[q];
        
        create_and_write_dataset(serialized, fid, file_dataspace, memory_dataspace, data_path);
        
    }

   
    dataspace_close(memory_dataspace);
    dataspace_close(file_dataspace);
    file_close(fid);

    delete [] serialized; serialized = NULL;

}                    


void Hdf5Writer::create_and_write_dataset(double* data, hid_t file_id, hid_t file_dataspace, hid_t memory_dataspace, std::string dataset_name){

    
    //This part is important as it does some kind of optimization on the data transfer,
    //without it, writing takes ages
    hid_t plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);


    hid_t dataset = H5Dcreate(file_id, dataset_name.c_str(), H5T_NATIVE_DOUBLE, file_dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t err = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, memory_dataspace, file_dataspace, plist_id, &(data[0]));
    
    if (err == FAIL) throw H5WriteError(dataset_name.c_str(), "asdf");
    dataset_close(dataset);
    memory_close(plist_id);
     

}


void Hdf5Writer::serialize_4d_array_at_index(const Array4d& array, double* serialized, size_t idx){


    #pragma omp parallel
	{

    

    //inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = array.i_begin(); i < array.i_end(); ++i){
	for (size_t j = array.j_begin(); j < array.j_end(); ++j){
	for (size_t k = array.k_begin(); k < array.k_end(); ++k){

        size_t ii = i - array.ngc();
        size_t jj = j - array.ngc();
        size_t kk = k - array.ngc();

        size_t serial_idx = (ii * array.ny() * array.nz()) + (jj * array.nz()) + kk;  

        serialized[serial_idx] = array(i, j, k, idx);


    }}}
    }//pragma    


}



void Hdf5Writer::dataspace_close(hid_t data_id) const{
    herr_t ret = H5Sclose(data_id);
    if (ret == FAIL) throw H5FileCloseError("data id");
}


void Hdf5Writer::file_close(hid_t file_id) const{

    herr_t ret = H5Fclose(file_id);
    if (ret == FAIL) throw H5FileCloseError("file");

}

void Hdf5Writer::group_close(hid_t group_id) const{

    herr_t ret = H5Gclose(group_id);
    if (ret == FAIL) throw H5FileCloseError("group");

}

void Hdf5Writer::memory_close(hid_t memory_id) const{

    herr_t ret = H5Pclose(memory_id);
    if (ret == FAIL) throw H5FileCloseError("memory");

}

void Hdf5Writer::dataset_close(hid_t dataset_id) const{

    herr_t ret = H5Dclose(dataset_id);
    if (ret == FAIL) throw H5FileCloseError("dataset");

}



void Hdf5Writer::write_mesh(Domain* d) const{


    //h5ex_t_array.c  CHECK THIS EXAMPLE!!

/*

    https://github.com/mvanmoer/HDF5_examples/blob/master/rectilineargrids/scalar/scalar.cpp


    The datatype, dataspace and dataset objects should be released once they are no longer needed by a program. Since each is an independent object, the must be released (or closed) separately. The following lines of code close the datatype, dataspace, and datasets that were created in the preceding section.

H5Tclose(datatype);

H5Dclose(dataset);

H5Sclose(dataspace); 

*/

}

void Hdf5Writer::write_fields(Domain* d, int iter) const{




}