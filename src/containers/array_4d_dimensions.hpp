#ifndef array_4d_dimensions_hh
#define array_4d_dimensions_hh

#include <cstddef> //size_t
////////////////////////////////////////////////////////////////////////////////
///
///		\brief 			helper to store array indices arr[NX][NY][NZ][nq]
///     
///		\param nnx, nny, nnz	    (Cartesian) Dimensions without barrier nodes 
///	   	\param nnq	            Dimension of the first subarray (typically a flux vector)
///     \param nngc	            Number of barrier cells		
///
////////////////////////////////////////////////////////////////////////////////
struct Array4dDimensions{

    //default
    Array4dDimensions() = default;

    //copy
    Array4dDimensions(const Array4dDimensions& ad) {  nx = ad.nx; ny = ad.ny; nz = ad.nz;
                                                    nq = ad.nq; ngc = ad.ngc; 
                                                    NX = ad.NX; NY = ad.NY; NZ = ad.NZ;
                                                 }   
        

    ////////////////////////////////////////////////////////////////////////////////
    ///
    ///		\brief 			helper to store array indices arr[NX][NY][NZ][nq]
    ///     
    ///		\param nnx, nny, nnz	    (Cartesian) Dimensions without barrier nodes 
    ///	   	\param nnq	            Dimension of the first subarray (typically a flux vector)
    ///     \param nngc	            Number of barrier cells		
    ///
    ////////////////////////////////////////////////////////////////////////////////
    Array4dDimensions(size_t nnx, size_t nny, size_t nnz, size_t nnq, size_t nngc){
        nx = nnx; ny = nny; nz = nnz; 
        nq = nnq; ngc = nngc;
        NX = nnx + 2*nngc; 
        NY = nny + 2*nngc;
        NZ = nnz + 2*nngc;
    }

    size_t nx, ny, nz, nq, ngc, NX, NY, NZ;


    
};



#endif