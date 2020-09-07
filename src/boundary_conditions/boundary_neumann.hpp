#ifndef BoundaryNeumann_hh
#define BoundaryNeumann_hh

#include "boundary.hpp"




class Neumann : public Boundary {

	
		
	public:
	
		Neumann() : Boundary() {}	
				
					
		Neumann(std::string type, int location, const Array4dDimensions& ad) : Boundary(type, location, ad) 
		{
			m_first_domain_west = m_ngc;
			m_first_domain_east = m_gc_start_east - 1;
			m_first_domain_north = m_ngc;
			m_first_domain_south = m_gc_start_south - 1;
			m_first_domain_back = m_ngc;
			m_first_domain_front = m_gc_start_front - 1;
		}					

		Neumann(std::string type, int location, const Array4d& arr) : Boundary(type, location, arr) 
		{
			m_first_domain_west = m_ngc;
			m_first_domain_east = m_gc_start_east - 1;
			m_first_domain_north = m_ngc;
			m_first_domain_south = m_gc_start_south - 1;
			m_first_domain_back = m_ngc;
			m_first_domain_front = m_gc_start_front - 1;
		}				
				
									
		virtual void apply(Array4d& );

		
		


	private:
		void apply_west(Array4d& arr) const;
		void apply_east(Array4d& arr) const;
		void apply_north(Array4d& arr) const;
		void apply_south(Array4d& arr) const;
		void apply_front(Array4d& arr) const;
		void apply_back(Array4d& arr) const;
	
		int m_first_domain_west;
		int m_first_domain_east;
		int m_first_domain_north;
		int m_first_domain_south;
		int m_first_domain_back;
		int m_first_domain_front;

	
		
		
		
		
			


    
};



#endif
