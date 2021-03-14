#include "array_4d.hpp"

Array4d::Array4d()
    : m_dimensions(Array4dDimensions(0, 0, 0, 0, 0))
    , m_NX(0)
    , m_NY(0)
    , m_NZ(0)
    , m_NQ(0)
    , m_data(nullptr) {
    // empty
}

Array4d::Array4d(const Array4dDimensions& dimensions)
    : m_dimensions(dimensions)
    , m_NX(dimensions.NX)
    , m_NY(dimensions.NY)
    , m_NZ(dimensions.NZ)
    , m_NQ(dimensions.nq) {
    allocate();
    set_zero();
}

Array4d::Array4d(
    const size_t nx, const size_t ny, const size_t nz, const size_t nq, const size_t nbarriers)
    : Array4d(Array4dDimensions(nx, ny, nz, nq, nbarriers)) {
    // empty
}

Array4d::~Array4d() { deallocate(); }

void Array4d::allocate() { m_data = new double[m_NX * m_NY * m_NZ * m_NQ]; }

void Array4d::deallocate() {

    delete[] m_data;
    m_data = NULL;
}

MPI_Datatype Array4d::x_direction_slicer() const {
    // THESE SHOULD BE MORE OPTIMIZED!!!
    // transfers NQ * nz size contiguous blocks
    /*
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
    */
    size_t transfer_block_size = ngc() * ny() * nz() * NQ(); // total size of the halo barrier
    size_t count = transfer_block_size / NQ();               // contiguous in z-direction
    int*   blocklens = new int[count]; // size of each contiguous memory block NQ * nz
    int*   indices   = new int[count]; // start indices of the blocks

    size_t temp = 0;
    for (size_t i = 0; i < ngc(); i++) {
        for (size_t j = j_begin(); j < j_end(); j++) {
            for (size_t k = k_begin(); k < k_end(); k++) {

                indices[temp]   = get_idx(i, j, k, 0);
                blocklens[temp] = NQ();

                temp++;
            }
        }
    }

    MPI_Datatype slice;
    MPI_Type_indexed(count, blocklens, indices, MPI_DOUBLE, &slice);
    MPI_Type_commit(&slice);

    delete[] blocklens;
    delete[] indices;

    return slice;
}

MPI_Datatype Array4d::y_direction_slicer() const {

    // transfers NQ * nz size contiguous blocks
    /*
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
    */
    size_t transfer_block_size = nx() * ngc() * nz() * NQ(); // total size of the halo barrier
    size_t count = transfer_block_size / NQ();               // contiguous in z-direction
    int*   blocklens = new int[count]; // size of each contiguous memory block NQ * nz
    int*   indices   = new int[count]; // start indices of the blocks

    size_t temp = 0;
    for (size_t i = i_begin(); i < i_end(); i++) {
        for (size_t j = 0; j < ngc(); j++) {
            for (size_t k = k_begin(); k < k_end(); k++) {
                indices[temp]   = get_idx(i, j, k, 0);
                blocklens[temp] = NQ();

                temp++;
            }
        }
    }

    MPI_Datatype slice;
    MPI_Type_indexed(count, blocklens, indices, MPI_DOUBLE, &slice);
    MPI_Type_commit(&slice);

    delete[] blocklens;
    delete[] indices;

    return slice;
}

MPI_Datatype Array4d::z_direction_slicer() const {
    // transfers NQ size contiguous blocks

    size_t transfer_block_size = nx() * ny() * ngc() * NQ(); // total size of the halo barrier
    size_t count = transfer_block_size / NQ();               // contiguous in z-direction
    int*   blocklens = new int[count]; // size of each contiguous memory block NQ * nz
    int*   indices   = new int[count]; // start indices of the blocks

    size_t temp = 0;
    for (size_t i = i_begin(); i < i_end(); i++) {
        for (size_t j = j_begin(); j < j_end(); j++) {
            for (size_t k = 0; k < ngc(); k++) {

                indices[temp]   = get_idx(i, j, k, 0);
                blocklens[temp] = NQ();

                temp++;
            }
        }
    }

    MPI_Datatype slice;
    MPI_Type_indexed(count, blocklens, indices, MPI_DOUBLE, &slice);
    MPI_Type_commit(&slice);

    delete[] blocklens;
    delete[] indices;

    return slice;
}

void Array4d::set_zero() {

#pragma omp parallel
    {

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = I_BEGIN(); i < I_END(); i++) {
            for (size_t j = J_BEGIN(); j < J_END(); j++) {
                for (size_t k = K_BEGIN(); k < K_END(); k++) {
                    for (size_t q = Q_BEGIN(); q < Q_END(); q++) {

                        size_t idx  = get_idx(i, j, k, q);
                        m_data[idx] = 0.0;
                        // m_data[i,j,k,q] = 0.0;
                    }
                }
            }
        }
    } // pragma
}
