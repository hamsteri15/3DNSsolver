#include "centered_convection.hpp"





void CenteredConvection::compute_residual(Array4d& residual){

    /*
    #pragma omp parallel
	{

	Array_4d R   = residual.get_data();
    Array_4d F_dx = m_F_dx.get_data();
    Array_4d F_dy = m_F_dy.get_data();
    Array_4d F_dz = m_F_dz.get_data();

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = residual.i_begin(); i < residual.i_end(); i++){
	for (size_t j = residual.j_begin(); j < residual.j_end(); j++){
	for (size_t k = residual.k_begin(); k < residual.k_end(); k++){
	for (size_t q = residual.q_begin(); q < residual.q_end(); q++){		

        R[i][j][k][q] = F_dx[i][j][k][q] + F_dy[i][j][k][q] + F_dz[i][j][k][q];

        
	}}}} //i j k q
		

	}//pragma

    */

}