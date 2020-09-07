#include "array_3d.hpp"

Array3d::Array3d()
:m_dimensions(Array3dDimensions(0,0,0,0)), m_NX(0), m_NY(0), m_NZ(0),
m_data(nullptr)
{
    //empty
}

Array3d::Array3d(const Array3dDimensions& dimensions) 
:m_dimensions(dimensions), m_NX(dimensions.NX), m_NY(dimensions.NY), m_NZ(dimensions.NZ)
{ 
    allocate(); 
}

Array3d::Array3d(const Array4dDimensions& dimensions) 
:Array3d(dimensions.nx, dimensions.ny, dimensions.nz, dimensions.ngc)
{ 
    //empty 
}


Array3d::Array3d(const size_t nx, const size_t ny, const size_t nz, const size_t nbarriers)
: Array3d(Array3dDimensions(nx, ny, nz, nbarriers))
{
   //empty   
}
    

Array3d::~Array3d() { deallocate(); }


void Array3d::allocate(){
    
    m_data = new double[m_NX*m_NY*m_NZ] ;
   
}

void Array3d::deallocate(){

    delete [] m_data;
    m_data = NULL;
}




/*

MPI_Datatype Array4d::x_direction_slicer() const{

    //transfers NQ * nz size contiguous blocks 

    size_t transfer_block_size = ngc() * ny() * nz() * NQ(); //total size of the halo barrier
    size_t count = transfer_block_size / (nz() * NQ()); //contiguous in z-direction
    int* blocklens = new int[count]; //size of each contiguous memory block NQ * nz
    int* indices = new int[count]; //start indices of the blocks

    size_t temp = 0;
    for (size_t i = 0; i < ngc(); i++){
    for (size_t j = j_begin(); j < j_end(); j++){


        indices[temp] = get_idx(i,j,0,0);
        blocklens[temp] = NQ() * nz();
        
        temp++;

    }}

    MPI_Datatype slice;      
    MPI_Type_indexed(count, blocklens, indices, MPI_DOUBLE, &slice);                    
    MPI_Type_commit(&slice);

    delete [] blocklens;
    delete [] indices;

    return slice;

}

MPI_Datatype Array4d::y_direction_slicer() const{
        
    //transfers NQ * nz size contiguous blocks

    size_t transfer_block_size = nx() * ngc() * nz() * NQ(); //total size of the halo barrier
    size_t count = transfer_block_size / (nz() * NQ()); //contiguous in z-direction
    int* blocklens = new int[count]; //size of each contiguous memory block NQ * nz
    int* indices = new int[count]; //start indices of the blocks

    size_t temp = 0;
    for (size_t i = i_begin(); i < i_end(); i++){
    for (size_t j = 0; j < ngc(); j++){

        indices[temp] = get_idx(i,j,0,0);
        blocklens[temp] = NQ() * nz();
        
        temp++;

    }}


    MPI_Datatype slice;
    MPI_Type_indexed(count, blocklens, indices, MPI_DOUBLE, &slice);                    
    MPI_Type_commit(&slice);

    delete [] blocklens;
    delete [] indices;

    return slice;
        

}

MPI_Datatype Array4d::z_direction_slicer() const{    
    //transfers NQ size contiguous blocks

    size_t transfer_block_size = nx() * ny() * ngc() * NQ(); //total size of the halo barrier
    size_t count = transfer_block_size / NQ(); //contiguous in z-direction
    int* blocklens = new int[count]; //size of each contiguous memory block NQ * nz
    int* indices = new int[count]; //start indices of the blocks

    size_t temp = 0;
    for (size_t i = i_begin(); i < i_end(); i++){
    for (size_t j = j_begin(); j < j_end(); j++){
    for (size_t k = 0; k < ngc(); k++){

        indices[temp] = get_idx(i,j,k,0);
        blocklens[temp] = NQ();
        
        temp++;

    }}}


    MPI_Datatype slice;
    MPI_Type_indexed(count, blocklens, indices, MPI_DOUBLE, &slice);                    
    MPI_Type_commit(&slice);

    delete [] blocklens;
    delete [] indices;

    return slice;


}


*/