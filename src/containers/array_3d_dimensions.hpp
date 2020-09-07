#ifndef array_3d_dimensions_hh
#define array_3d_dimensions_hh

#include <cstddef> //size_t
#include "array_4d_dimensions.hpp"

////////////////////////////////////////////////////////////////////////////////
///
///		\brief 			helper to store array indices arr[NX][NY][NZ]
///     
///		\param nnx, nny, nnz	    (Cartesian) Dimensions without barrier nodes 
///     \param nngc	            Number of barrier cells		
///
////////////////////////////////////////////////////////////////////////////////
struct Array3dDimensions{

    //default
    Array3dDimensions() = default;

    //copy
    Array3dDimensions(const Array3dDimensions& ad) {  nx = ad.nx; ny = ad.ny; nz = ad.nz;
                                                     ngc = ad.ngc; 
                                                    NX = ad.NX; NY = ad.NY; NZ = ad.NZ;
                                                 }   


    //from 4d array dimensions (should probably be removed)
    Array3dDimensions(const Array4dDimensions& ad) {  nx = ad.nx; ny = ad.ny; nz = ad.nz;
                                                     ngc = ad.ngc; 
                                                    NX = ad.NX; NY = ad.NY; NZ = ad.NZ;
                                                 }   
        


    Array3dDimensions(size_t nnx, size_t nny, size_t nnz, size_t nngc){
        nx = nnx; ny = nny; nz = nnz; 
        NX = nnx + 2*nngc; 
        NY = nny + 2*nngc;
        NZ = nnz + 2*nngc;
    }

    size_t nx, ny, nz, ngc, NX, NY, NZ;


    
};



#endif