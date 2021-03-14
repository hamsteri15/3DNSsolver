#ifndef CdConstants_hh
#define CdConstants_hh
////////////////////////////////////////////////////////////////////////////////////////
//
// Stencil width, start index and first and second derivative coefficients
//
////////////////////////////////////////////////////////////////////////////////////////

// CD-2
static const int    cd2_stencil_width = 3;
static const int    cd2_start_idx     = -1;
static const double cd2_df_coeff[3]   = {-1. / 2, 0., 1. / 2.};
static const double cd2_ddf_coeff[3]  = {1., -2., 1.};

// CD-4
static const int    cd4_stencil_width = 5;
static const int    cd4_start_idx     = -2;
static const double cd4_df_coeff[5]   = {1. / 12., -2. / 3., 0., 2. / 3., -1. / 12.};
static const double cd4_ddf_coeff[5]  = {-1. / 12., 4. / 3., -5. / 2., 4. / 3., -1. / 12.};

// CD-6
static const int    cd6_stencil_width = 7;
static const int    cd6_start_idx     = -3;
static const double cd6_df_coeff[7]   = {
    -1. / 60., 3. / 20., -3. / 4., 0., 3. / 4., -3. / 20., 1. / 60.};
static const double cd6_ddf_coeff[7] = {
    1. / 90., -3. / 20., 3. / 2., -49. / 18., 3. / 2., -3. / 20., 1. / 90.};

#endif