#include "weno-5.hpp"



void Weno5::solve(const Grid& grid, const Array4d& primitive, const Array4d& conservative, Array4d& residual ) {


	ConvectionFlux flux; //move me to constructor

	Array4d f_x = Array4d(primitive.get_dimensions());
	Array4d f_y = Array4d(primitive.get_dimensions());
	Array4d f_z = Array4d(primitive.get_dimensions());

	
	f_x.set_zero();
	f_y.set_zero();
	f_z.set_zero();
	
	
	//x-dir
	Array4d F_x = flux.form_flux(primitive, m_gamma, 1.0, 0.0, 0.0);
	Array3d eig_x = flux.max_eigenvalue(primitive, m_gamma, 1.0, 0.0, 0.0);
	x_sweep(F_x, conservative, primitive, eig_x, f_x); 

	
	
	//y-dir
	Array4d F_y = flux.form_flux(primitive, m_gamma, 0.0, 1.0, 0.0);
	Array3d eig_y = flux.max_eigenvalue(primitive, m_gamma, 0.0, 1.0, 0.0);
	y_sweep(F_y, conservative, primitive, eig_y, f_y); //this is the Weno numerical flux 


	

	

	
	if (grid.is_3D()){

		//z-dir
		Array4d F_z = flux.form_flux(primitive, m_gamma, 0.0, 0.0, 1.0);
		Array3d eig_z = flux.max_eigenvalue(primitive, m_gamma, 0.0, 0.0, 1.0);
		z_sweep(F_z, conservative, primitive, eig_z, f_z); //this is the Weno numerical flux 

	}
	


	compute_residual(grid, f_x, f_y, f_z, residual);


	

}






void Weno5::compute_residual(const Grid& grid, const Array4d& f_x, const Array4d& f_y, const Array4d& f_z, Array4d& residual){

	
	
	#pragma omp parallel
	{

	double dx = grid.dx(); double dy = grid.dy(); double dz = grid.dz();

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = residual.i_begin(); i < residual.i_end(); i++){
	for (size_t j = residual.j_begin(); j < residual.j_end(); j++){
	for (size_t k = residual.k_begin(); k < residual.k_end(); k++){
	for (size_t q = residual.q_begin(); q < residual.q_end(); q++){			
		
			
			
		//x-dir (f_{i+1/2} - f_{i-1/2}) /dx
		residual(i,j,k,q) +=  (f_x(i, j, k, q) - f_x(i-1, j, k, q))/dx;

		//y-dir (f_{j+1/2} - f_{j-1/2}) /dy
		residual(i,j,k,q) += (f_y(i, j, k, q) - f_y(i, j-1, k, q))/dy;

		//z-dir (f_{k+1/2} - f_{k-1/2}) /dz
		residual(i,j,k,q) += (f_z(i, j, k, q) - f_z(i, j, k-1, q))/dz;
	
	}}}}

	}//pragma
	

}	


void Weno5::x_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f){

	

	#pragma omp parallel
	{

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = F.i_begin() - 1;  i < F.i_end(); i++){
	for (size_t j = F.j_begin(); 	  j < F.j_end(); j++){
	for (size_t k = F.k_begin(); 	  k < F.k_end(); k++){
				
			//max eigenvalue in the stencil
			double alpha = get_max_alpha(	eig(i-2,j,k) , 
										    eig(i-1,j,k), 
										    eig(i,  j,k), 
											eig(i+1,j,k), 
											eig(i+2,j,k) );

			for (size_t q = F.q_begin(); q < F.q_end() ; q++){

				double fL_m2 = ( F(i-2, j, k, q) + alpha * U(i-2, j, k, q) ) * 0.5;
				double fR_m2 = ( F(i-1, j, k, q) - alpha * U(i-1, j, k, q) ) * 0.5;

				double fL_m1 = ( F(i-1, j, k, q) + alpha * U(i-1, j, k, q) ) * 0.5;
				double fR_m1 = ( F(i  , j, k, q) - alpha * U(i  , j, k, q) ) * 0.5;

				double fL_ = ( F(i  , j, k, q) + alpha * U(i  , j, k, q) ) * 0.5;
				double fR_ = ( F(i+1, j, k, q) - alpha * U(i+1, j, k, q) ) * 0.5;

				double fL_p1 = ( F(i+1, j, k, q) + alpha * U(i+1, j, k, q) ) * 0.5;
				double fR_p1 = ( F(i+2, j, k, q) - alpha * U(i+2, j, k, q) ) * 0.5;

				double fL_p2 = ( F(i+2, j, k, q) + alpha * U(i+2, j, k, q) ) * 0.5;
				double fR_p2 = ( F(i+3, j, k, q) - alpha * U(i+3, j, k, q) ) * 0.5;

				double f_left = WENO5_left(fL_m2, fL_m1, fL_, fL_p1, fL_p2);
				double f_right = WENO5_right(fR_m2, fR_m1, fR_, fR_p1, fR_p2);

				f(i,j,k,q) = f_left + f_right;

			}//q

	}}}	//kji

	}//pragma



}

void Weno5::y_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f){

	

	#pragma omp parallel
	{

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = F.i_begin();     i < F.i_end(); i++){
	for (size_t j = F.j_begin()-1; 	 j < F.j_end(); j++){
	for (size_t k = F.k_begin(); 	 k < F.k_end(); k++){
				
			//max eigenvalue in the stencil
			double alpha = get_max_alpha(	eig(i,j-2,k) , 
										    eig(i,j-1,k), 
										    eig(i,j,k), 
											eig(i,j+1,k), 
											eig(i,j+2,k) );

			for (size_t q = F.q_begin(); q < F.q_end() ; q++){

				double fL_m2 = ( F(i, j-2, k, q) + alpha * U(i, j-2, k, q) ) * 0.5;
				double fR_m2 = ( F(i, j-1, k, q) - alpha * U(i, j-1, k, q) ) * 0.5;

				double fL_m1 = ( F(i, j-1, k, q) + alpha * U(i, j-1, k, q) ) * 0.5;
				double fR_m1 = ( F(i  , j, k, q) - alpha * U(i  , j, k, q) ) * 0.5;

				double fL_ = ( F(i  , j, k, q) + alpha * U(i, j,   k, q) ) * 0.5;
				double fR_ = ( F(i, j+1, k, q) - alpha * U(i, j+1, k, q) ) * 0.5;

				double fL_p1 = ( F(i, j+1, k, q) + alpha * U(i, j+1, k, q) ) * 0.5;
				double fR_p1 = ( F(i, j+2, k, q) - alpha * U(i, j+2, k, q) ) * 0.5;

				double fL_p2 = ( F(i, j+2, k, q) + alpha * U(i, j+2, k, q) ) * 0.5;
				double fR_p2 = ( F(i, j+3, k, q) - alpha * U(i, j+3, k, q) ) * 0.5;

				double f_left = WENO5_left(fL_m2, fL_m1, fL_, fL_p1, fL_p2);
				double f_right = WENO5_right(fR_m2, fR_m1, fR_, fR_p1, fR_p2);

				f(i,j,k,q) = f_left + f_right;

			}//q

	}}}	//kji

	}//pragma



}


void Weno5::z_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f){


	#pragma omp parallel
	{

	//inner cells loop	
	#pragma omp for	schedule(dynamic)
	for (size_t i = F.i_begin();     i < F.i_end(); i++){
	for (size_t j = F.j_begin(); 	 j < F.j_end(); j++){
	for (size_t k = F.k_begin() - 1; k < F.k_end(); k++){
				
			//max eigenvalue in the stencil
			double alpha = get_max_alpha(	eig(i,j,k-2) , 
										    eig(i,j,k-1), 
										    eig(i,j,k), 
											eig(i,j,k+1), 
											eig(i,j,k+2) );

			for (size_t q = F.q_begin(); q < F.q_end() ; q++){

				double fL_m2 = ( F(i, j, k-2, q) + alpha * U(i, j, k-2, q) ) * 0.5;
				double fR_m2 = ( F(i, j, k-1, q) - alpha * U(i, j, k-1, q) ) * 0.5;

				double fL_m1 = ( F(i, j, k-1, q) + alpha * U(i, j, k-1, q) ) * 0.5;
				double fR_m1 = ( F(i, j, k, q) - alpha * U(i  , j, k, q) ) * 0.5;

				double fL_ = ( F(i, j, k, q) + alpha * U(i  , j, k, q) ) * 0.5;
				double fR_ = ( F(i, j, k+1, q) - alpha * U(i, j, k+1, q) ) * 0.5;

				double fL_p1 = ( F(i, j, k+1, q) + alpha * U(i, j, k+1, q) ) * 0.5;
				double fR_p1 = ( F(i, j, k+2, q) - alpha * U(i, j, k+2, q) ) * 0.5;

				double fL_p2 = ( F(i, j, k+2, q) + alpha * U(i, j, k+2, q) ) * 0.5;
				double fR_p2 = ( F(i, j, k+3, q) - alpha * U(i, j, k+3, q) ) * 0.5;

				double f_left = WENO5_left(fL_m2, fL_m1, fL_, fL_p1, fL_p2);
				double f_right = WENO5_right(fR_m2, fR_m1, fR_, fR_p1, fR_p2);

				f(i,j,k,q) = f_left + f_right;

			}//q

	}}}	//kji

	}//pragma


}









template<typename T>
std::array<T, 3> Weno5::WENO5_weights_shu(T f_m2, T f_m1, T f_, T f_p1, T f_p2, const double d[3]){

	
	

	T temp1; T temp2;
	T b0; T b1; T b2;	

	
	 
	temp1 = f_m2 - 2.*f_m1 + f_;
	temp2 = f_m2 - 4.*f_m1 + 3.*f_;
	b0 = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

	temp1 = f_m1 - 2.*f_ + f_p1;
	temp2 = f_m1 - f_p1;
	b1 = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

	
	temp1 = f_ - 2.*f_p1 + f_p2;
	temp2 = 3.*f_ -4.*f_p1 + f_p2;
	b2 = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;	

	
	T alpha_0 = d[0]/((epsilon_shu + b0)*(epsilon_shu + b0));
	T alpha_1 = d[1]/((epsilon_shu + b1)*(epsilon_shu + b1));
	T alpha_2 = d[2]/((epsilon_shu + b2)*(epsilon_shu + b2));
	
	T alpha = alpha_0 + alpha_1 + alpha_2;

	
	return {alpha_0/alpha, alpha_1/alpha, alpha_2/alpha};

	

}

template<typename T>
std::array<T, 3> Weno5::WENO5_weights_borges(T f_m2, T f_m1, T f_, T f_p1, T f_p2, const double d[3]){

	

	T temp1; T temp2;
	T b0; T b1; T b2;	

	
	 
	temp1 = f_m2 - 2.*f_m1 + f_;
	temp2 = f_m2 - 4.*f_m1 + 3.*f_;
	b0 = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

	temp1 = f_m1 - 2.*f_ + f_p1;
	temp2 = f_m1 - f_p1;
	b1 = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

	
	temp1 = f_ - 2.*f_p1 + f_p2;
	temp2 = 3.*f_ -4.*f_p1 + f_p2;
	b2 = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;	

	T tau5 = std::abs(b0 - b2); //make sure this is defined for T


    T alpha_0 = d[0] * (1.0 + (tau5 / (b0 + epsilon_borges)));
    T alpha_1 = d[1] * (1.0 + (tau5 / (b1 + epsilon_borges)));
    T alpha_2 = d[2] * (1.0 + (tau5 / (b2 + epsilon_borges)));

	
	T alpha = alpha_0 + alpha_1 + alpha_2;
	
	
	return {alpha_0/alpha, alpha_1/alpha, alpha_2/alpha};



}


template <typename T>
T Weno5::WENO5_right(T fR_m2, T fR_m1, T fR_, T fR_p1, T fR_p2){


	//Eno stencils
	T f0 = (-fR_m2 + 5.*fR_m1 + 2.*fR_)*onepersix;
	T f1 = (2.*fR_m1 + 5.*fR_ - fR_p1)*onepersix;
	T f2 = (11.*fR_ - 7.*fR_p1 + 2.*fR_p2)*onepersix;
	
	std::array<T,3> weights;

	if (m_weight_type == "Borges"){
		weights = WENO5_weights_borges(fR_m2, fR_m1, fR_, fR_p1, fR_p2, d_right);	
	}
	else{
		weights = WENO5_weights_shu(fR_m2, fR_m1, fR_, fR_p1, fR_p2, d_right);
	}
	

	T FRm05 = weights[0]*f0 + weights[1]*f1 + weights[2]*f2;
	

	return FRm05;


}


template<typename T>
T Weno5::WENO5_left(T fL_m2, T fL_m1, T fL_, T fL_p1, T fL_p2){


	
	//ENO-stencils
	T f0 = (2.*fL_m2 - 7.*fL_m1 + 11.*fL_)*onepersix;
	T f1 = (-fL_m1 + 5.*fL_ + 2.*fL_p1)*onepersix;
	T f2 = (2.*fL_ + 5.*fL_p1 - fL_p2)*onepersix;
	
	std::array<T,3> weights;
	if (m_weight_type == "Borges"){
		weights = WENO5_weights_borges(fL_m2, fL_m1, fL_, fL_p1, fL_p2, d_left);
	}
	else{
		weights = WENO5_weights_shu(fL_m2, fL_m1, fL_, fL_p1, fL_p2, d_left);
	}

	T FLp05 = weights[0]*f0 + weights[1]*f1 + weights[2]*f2;

	return FLp05;



}


double Weno5::get_max_alpha(double d1, double d2, double d3, double d4, double d5){

	double temp1 = std::max(d1,d2);
	double temp2 = std::max(temp1, d3);
	double temp3 = std::max(temp2, d4);
	return std::max(temp3, d5);


}
