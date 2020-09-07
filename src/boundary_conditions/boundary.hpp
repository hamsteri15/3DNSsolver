#ifndef Boundary_hh
#define Boundary_hh

#include <iostream>
#include "params.hpp"
#include "containers/array_4d.hpp"





class Boundary {

	

		
	public:
	
					
							
		Boundary() = default;
	
		//from array dimensions
		Boundary(std::string type, int location, const Array4dDimensions& ad);
		
		//from array					
		Boundary(std::string type, int location, const Array4d& arr);							
		
		virtual void apply(Array4d& ) {};

		
		//void apply_Neumann(node5_array3d& );
		

		inline std::string get_type() {return m_type;};



	protected:

	
		std::string m_type;
		
		int m_location, m_ngc;
		
		int m_nx, m_ny, m_nz;
		
		

		int m_gc_start_west;
		int m_gc_start_east;

		int m_gc_start_north;
		int m_gc_start_south;
		
		int m_gc_start_front;
		int m_gc_start_back;

		


		

		


		int m_q_start, m_q_end;

		//virtual void get_domain_data() {};
		
		//virtual void set_domain_data() {};
			
		
		
		
			


    
};



#endif
