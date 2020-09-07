
#include "weno-5cw.hpp"



void Weno5cw::compute_num_flux(	const double fL[5][5], 
								const double fR[5][5], 
							  	const double L[5][5], const double R[5][5], 
								double* flux){


	
	double fL_[5][5];
	matrix::square_matrix_matrix_product(L[0], fL[0], fL_[0], 5);
	
	double fR_[5][5]; 
	matrix::square_matrix_matrix_product(L[0], fR[0], fR_[0], 5);

	double f_temp[5]; 
	
	//size_t nq = m_array_dimensions.nq;

	for (int q = 0; q < 5; q++){
	
		double f_left = WENO5_left(fL_[q][0], fL_[q][1], fL_[q][2], fL_[q][3], fL_[q][4]);
		double f_right = WENO5_right(fR_[q][0], fR_[q][1], fR_[q][2], fR_[q][3], fR_[q][4]);
		f_temp[q] = f_left + f_right;
		

	}
	
	
	matrix::square_matrix_vector_product(R[0], f_temp, flux, 5);
		
}




				
void Weno5cw::x_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f) {


	#pragma omp parallel
	{
	

	
	//left and right biased stencils
	double fL[5][5]; double fR[5][5]; 

	//left and right eigenmatrices
	double L[5][5]; double R[5][5];
	
	ConvectionFlux flux;
	
	std::vector<double> n = {1.0, 0.0, 0.0}; //unit normal
	std::vector<double> orthog = flux.orthogonal_unit_vectors(n);
	std::vector<double> l = {orthog[0], orthog[1], orthog[2] };
	std::vector<double> m = {orthog[3], orthog[4], orthog[5] };

	double f_local[5]; //local weno flux

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = U.i_begin() - 1;  i < U.i_end(); i++){
	for (size_t j = U.j_begin();      j < U.j_end(); j++){
	for (size_t k = U.k_begin();      k < U.k_end(); k++){
				
				
		
				//max eigenvalue in the stencil
				double alpha = get_max_alpha(	eig(i-2,j,k) , 
										    eig(i-1,j,k), 
										    eig(i,  j,k), 
											eig(i+1,j,k), 
											eig(i+2,j,k) );

				
				//compute the eigensystem at i+1/2
				double W_avg[5];
				for (size_t q = W.q_begin(); q < W.q_end(); q++){
					W_avg[q] = 0.5 * (W(i,j,k,q) + W(i + 1,j,k,q));
				}

				
				flux.eigenvectors( W_avg[0], W_avg[1], W_avg[2], W_avg[3], W_avg[4], m_gamma, n, l, m, L[0], R[0] );				  
				
				//fill up left and right biased stencils, ii is the stencil width
				for (size_t ii = 0; ii < 5; ii++){

					size_t idx = i - 2 + ii;

					for (size_t q = F.q_begin(); q < F.q_end() ; q++){

						//this is the flux-split
						fL[q][ii] = (F(idx, j, k, q) 	 + alpha*U(idx, j, k, q   )  )*0.5;
						fR[q][ii] = (F(idx + 1, j, k, q) - alpha*U(idx + 1, j, k, q) )*0.5;
						
					}//q
				
				} //stencil
				
				
				
				compute_num_flux(fL, fR, L, R, f_local);
				
				for (size_t q = W.q_begin(); q < W.q_end(); q++){
					f(i,j,k,q) = f_local[q];
				}
				

				
				
	}}} //kji
	
	}//pragma
	

}		
void Weno5cw::y_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f) {

	

	#pragma omp parallel
	{
	

	
	//left and right biased stencils
	double fL[5][5]; double fR[5][5]; 

	//left and right eigenmatrices
	double L[5][5]; double R[5][5];
	
	ConvectionFlux flux;
	
	std::vector<double> n = {0.0, 1.0, 0.0}; //unit normal
	std::vector<double> orthog = flux.orthogonal_unit_vectors(n);
	std::vector<double> l = {orthog[0], orthog[1], orthog[2] };
	std::vector<double> m = {orthog[3], orthog[4], orthog[5] };

	double f_local[5]; //local weno flux

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = U.i_begin();      i < U.i_end(); i++){
	for (size_t j = U.j_begin() - 1;  j < U.j_end(); j++){
	for (size_t k = U.k_begin();      k < U.k_end(); k++){
				
				
		
				//max eigenvalue in the stencil
				double alpha = get_max_alpha(	eig(i,j-2,k) , 
										    eig(i,j-1,k), 
										    eig(i,  j,k), 
											eig(i,j+1,k), 
											eig(i,j+2,k) );

				
				//compute the eigensystem at i+1/2
				double W_avg[5];
				for (size_t q = W.q_begin(); q < W.q_end(); q++){
					W_avg[q] = 0.5 * (W(i,j,k,q) + W(i,j + 1,k,q));
				}
				
				
				flux.eigenvectors( W_avg[0], W_avg[1], W_avg[2], W_avg[3], W_avg[4], m_gamma, n, l, m, L[0], R[0] );				  
				//fill up left and right biased stencils, ii is the stencil width
				for (size_t ii = 0; ii < 5; ii++){

					size_t idx = j - 2 + ii;

					for (size_t q = F.q_begin(); q < F.q_end() ; q++){

						//this is the flux-split
						fL[q][ii] = (F(i, idx,   k, q) 	 + alpha*U(i, idx,   k, q   ))*0.5;
						fR[q][ii] = (F(i, idx+1, k, q)   - alpha*U(i, idx+1, k, q   ))*0.5;
						
					}//q
				
				} //stencil
				
				
				
				compute_num_flux(fL, fR, L, R, f_local);
				
				for (size_t q = W.q_begin(); q < W.q_end(); q++){
					f(i,j,k,q) = f_local[q];
				}
				

				
				
	}}} //kji
	
	}//pragma

	

}
void Weno5cw::z_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f) {

	#pragma omp parallel
	{
	

	
	//left and right biased stencils
	double fL[5][5]; double fR[5][5]; 

	//left and right eigenmatrices
	double L[5][5]; double R[5][5];
	
	ConvectionFlux flux;
	
	std::vector<double> n = {0.0, 0.0, 1.0}; //unit normal
	std::vector<double> orthog = flux.orthogonal_unit_vectors(n);
	std::vector<double> l = {orthog[0], orthog[1], orthog[2] };
	std::vector<double> m = {orthog[3], orthog[4], orthog[5] };

	double f_local[5]; //local weno flux

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = U.i_begin();      i < U.i_end(); i++){
	for (size_t j = U.j_begin();  	  j < U.j_end(); j++){
	for (size_t k = U.k_begin() - 1;  k < U.k_end(); k++){
				
				
		
				//max eigenvalue in the stencil
				double alpha = get_max_alpha(	eig(i,j,k-2) , 
										    	eig(i,j,k-1), 
										    	eig(i,  j,k), 
												eig(i,j,k+1), 
												eig(i,j,k+2) );

				
				//compute the eigensystem at i+1/2
				double W_avg[5];
				for (size_t q = W.q_begin(); q < W.q_end(); q++){
					W_avg[q] = 0.5 * (W(i,j,k,q) + W(i,j,k + 1,q));
				}
				
				flux.eigenvectors( W_avg[0], W_avg[1], W_avg[2], W_avg[3], W_avg[4], m_gamma, n, l, m, L[0], R[0] );				  
				
				//fill up left and right biased stencils, ii is the stencil width
				for (size_t ii = 0; ii < 5; ii++){

					size_t idx = k - 2 + ii;

					for (size_t q = F.q_begin(); q < F.q_end() ; q++){

						//this is the flux-split
						fL[q][ii] = (F(i, j, idx, 	  q) 	 + alpha*U(i, j, idx,   q   ))*0.5;
						fR[q][ii] = (F(i, j, idx + 1, q)   	 - alpha*U(i, j, idx+1, q   ))*0.5;
						
					}//q
				
				} //stencil
				
				
				
				compute_num_flux(fL, fR, L, R, f_local);
				
				for (size_t q = W.q_begin(); q < W.q_end(); q++){
					f(i,j,k,q) = f_local[q];
				}
				

				
				
	}}} //kji
	
	}//pragma




}

/*

void Weno5cw::x_sweep(Array& conservative, Array& primitive){
	
	

	#pragma omp parallel
	{
	

	//data pointers
	Array_4d eig = m_max_eig_x.get_data();
	Array_4d F   = m_Euler_flux_x.get_data();
	Array_4d f   = m_num_flux_x.get_data();
	Array_4d U     = conservative.get_data();
	Array_4d W     = primitive.get_data();


	//left and right biased stencils
	double fL[5][5]; double fR[5][5]; 

	//left and right eigenmatrices
	double L[5][5]; double R[5][5];
	
	ConvectionFlux flux;
	

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = conservative.i_begin() - 1;  i < conservative.i_end(); i++){
	for (size_t j = conservative.j_begin();      j < conservative.j_end(); j++){
	for (size_t k = conservative.k_begin();      k < conservative.k_end(); k++){
				
		
				//max eigenvalue in the stencil
				double alpha = get_max_alpha(	eig[i-2][j][k][0], 
													eig[i-1][j][k][0], 
													eig[i  ][j][k][0], 
													eig[i+1][j][k][0], 
													eig[i+2][j][k][0]);


				//compute the eigensystem at i+1/2
				double W_avg[5];
				computeAverage(W[i][j][k], W[i+1][j][k], W_avg);
				flux.eigensystem(W_avg, L[0], R[0], m_gamma, 1);

				
				
				//fill up left and right biased stencils, ii is the stencil width
				for (size_t ii = 0; ii < 5; ii++){

					size_t idx = i - 2 + ii;

					for (size_t q = conservative.q_begin(); q < conservative.q_end() ; q++){

						//this is the flux-split
						fL[q][ii] = (F[idx  ][j][k][q] + alpha*U[idx  ][j][k][q])*0.5;
						fR[q][ii] = (F[idx+1][j][k][q] - alpha*U[idx+1][j][k][q])*0.5;
						
					}//q
				
				} //stencil
				compute_num_flux(fL, fR, L, R, f[i][j][k]);
				
	}}} //kji
	
	}//pragma
	

}

void Weno5cw::y_sweep(Array& conservative, Array& primitive){

	
	#pragma omp parallel
	{

	//data pointers
	Array_4d eig = m_max_eig_y.get_data();
	Array_4d F   = m_Euler_flux_y.get_data();
	Array_4d f   = m_num_flux_y.get_data();
	Array_4d U     = conservative.get_data();
	Array_4d W     = primitive.get_data();


	//left and right biased stencils
	double fL[5][5]; double fR[5][5]; 

	//left and right eigenmatrices
	double L[5][5]; double R[5][5];

	ConvectionFlux flux;

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = conservative.i_begin();      i < conservative.i_end(); i++){
	for (size_t j = conservative.j_begin() - 1;  j < conservative.j_end(); j++){
	for (size_t k = conservative.k_begin(); 	 k < conservative.k_end(); k++){

			double alpha = get_max_alpha(	eig[i][j-2][k][0], 
											eig[i][j-1][k][0], 
											eig[i][j  ][k][0], 
											eig[i][j+1][k][0], 
											eig[i][j+2][k][0]);


				//compute the eigensystem
				double W_avg[5];
				computeAverage(W[i][j][k], W[i][j+1][k], W_avg);
				flux.eigensystem(W_avg, L[0], R[0], m_gamma, 2);

				
				
				//fill up left and right biased stencils
				for (size_t jj = 0; jj < 5; jj++){

					size_t idx = j - 2 + jj;
					for (size_t q = conservative.q_begin(); q < conservative.q_end() ; q++){
				
						//this is the flux-split
						fL[q][jj] = (F[i][idx  ][k][q] + alpha*U[i][idx  ][k][q])*0.5;
						fR[q][jj] = (F[i][idx+1][k][q] - alpha*U[i][idx+1][k][q])*0.5;
						
					}
				
				}
				compute_num_flux(fL, fR, L, R, f[i][j][k]);
				
	}}} //kji

	}//pragma
	
	
}

void Weno5cw::z_sweep(Array& conservative, Array& primitive){

	#pragma omp parallel
	{

	//data pointers
	Array_4d eig = m_max_eig_z.get_data();
	Array_4d F   = m_Euler_flux_z.get_data();
	Array_4d f   = m_num_flux_z.get_data();
	Array_4d U     = conservative.get_data();
	Array_4d W     = primitive.get_data();
	//left and right biased stencils
	double fL[5][5]; double fR[5][5]; 

	//left and right eigenmatrices
	double L[5][5]; double R[5][5];

	ConvectionFlux flux;

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = conservative.i_begin();      i < conservative.i_end(); i++){
	for (size_t j = conservative.j_begin();      j < conservative.j_end(); j++){
	for (size_t k = conservative.k_begin() - 1;  k < conservative.k_end(); k++){

				double alpha = get_max_alpha(	eig[i][j][k-2][0], 
													eig[i][j][k-1][0], 
													eig[i][j][k  ][0], 
													eig[i][j][k+1][0], 
													eig[i][j][k+2][0]);


				//compute the eigensystem at i+1/2
				double W_avg[5];
				computeAverage(W[i][j][k], W[i][j][k+1], W_avg);
				flux.eigensystem(W_avg, L[0], R[0], m_gamma, 3);

				
				
				//fill up left and right biased stencils
				for (size_t kk = 0; kk < 5; kk++){

					size_t idx = k - 2 + kk;
					for (size_t q = conservative.q_begin(); q < conservative.q_end() ; q++){

						//this is the flux-split
						fL[q][kk] = (F[i][j][idx  ][q] + alpha*U[i][j][idx  ][q])*0.5;
						fR[q][kk] = (F[i][j][idx+1][q] - alpha*U[i][j][idx+1][q])*0.5;
						
					}
				
				}
				compute_num_flux(fL, fR, L, R, f[i][j][k]);
				
	}}} //kji

	}//pragma

	

}

*/



