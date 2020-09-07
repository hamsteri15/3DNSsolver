#include "boundary_neumann.hpp"




void Neumann::apply(Array4d& arr){


	switch(m_location) {
	
		case 0: apply_west(arr); break; //west
        case 1: apply_east(arr); break; //east   
        case 2: apply_north(arr); break; //north
        case 3: apply_south(arr); break; //south
        case 4: apply_front(arr); break; //front           
        case 5: apply_back(arr); break; //back 
		default: {
		
				std::cout << "Wrong m_location in Boundary::Neumann" << std::endl;
				std::cout << "Exiting" << std::endl;
				std::exit(1);
		
		
		}
	
	}



} 


void Neumann::apply_west(Array4d& arr) const {

	//normal
	#pragma omp parallel
	{

	#pragma omp for	schedule(dynamic)
	for (int i = 0; i < m_ngc; i++){	
		for (int j = m_first_domain_north; j < m_gc_start_south; j++){
			for (int k = m_first_domain_back; k < m_gc_start_front; k++){
				for (int q = m_q_start; q < m_q_end ; q++){
				
				
					arr(m_gc_start_west + i, j, k, q) = arr(m_first_domain_west,j,k,q);

				
				}
			}
		}
	}
	}//pragma

	
	
	
		

}
void Neumann::apply_east(Array4d& arr) const {


	#pragma omp parallel
	{

	#pragma omp for	schedule(dynamic)
	for (int i=0; i < m_ngc; i++){
		for (int j = m_ngc; j<m_ny + m_ngc; j++){
			for (int k = m_ngc; k < m_nz + m_ngc; k++){
				for (int q = m_q_start; q < m_q_end ; q++){
					
					arr(m_gc_start_east + i, j, k, q) = arr(m_first_domain_east, j, k, q);
					
					
				}
					
			}
		}
		
	}
	}//pragma


	
	

}

void Neumann::apply_north(Array4d& arr) const {

	
	#pragma omp parallel
	{

	#pragma omp for	schedule(dynamic)
	for (int i=m_ngc; i<m_nx + m_ngc; i++){	//domain replace
		for (int j = 0; j<m_ngc; j++){
			for (int k = m_ngc; k<m_nz + m_ngc; k++){ //domain replace
				for (int q = m_q_start; q < m_q_end ; q++){

					arr(i, m_gc_start_north + j, k, q) = arr(i, m_first_domain_north, k, q);
					
				}
			}
		}
	}
	}//pragma
	
	

	


}
void Neumann::apply_south(Array4d& arr) const {

	#pragma omp parallel
	{

	#pragma omp for	schedule(dynamic)
	for (int i=m_ngc; i<m_nx + m_ngc; i++){	//domain replace
		for (int j = 0; j<m_ngc; j++){
			for (int k = m_ngc; k<m_nz + m_ngc; k++){ //domain replace
				for (int q = m_q_start; q < m_q_end ; q++){

					arr(i, m_gc_start_south + j, k, q) = arr(i, m_first_domain_south, k, q);
					
				}
			}
		}
	}
	}//pragma
	
	
	

}

void Neumann::apply_front(Array4d& arr) const {

	#pragma omp parallel
	{

	#pragma omp for	schedule(dynamic)
	for (int i=m_ngc; i<m_nx + m_ngc; i++){	//domain replace
		for (int j = m_ngc; j < m_ny + m_ngc; j++){ //domain replace
			for (int k = 0; k < m_ngc; k++){ 
				for (int q = m_q_start; q < m_q_end ; q++){

					arr(i, j, m_gc_start_front + k, q) = arr(i, j, m_first_domain_front, q);
					
				}
			}
		}
	}
	}//pragma
	
	
	


}
void Neumann::apply_back(Array4d& arr) const {

	#pragma omp parallel
	{

	#pragma omp for	schedule(dynamic)
	for (int i=m_ngc; i<m_nx + m_ngc; i++){	//domain replace
		for (int j = m_ngc; j < m_ny + m_ngc; j++){ //domain replace
			for (int k = 0; k < m_ngc; k++){ 
				for (int q = m_q_start; q < m_q_end ; q++){

					arr(i, j, m_gc_start_back + k, q) = arr(i, j, m_first_domain_back, q);
					
				}
			}
		}
	}
	}//pragma
	
	
	
	


}
