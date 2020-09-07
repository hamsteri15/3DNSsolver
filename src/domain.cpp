#include "domain.hpp"



Domain::Domain(const Params& p, const Decomposition& dec)
:
m_process_rank(dec.get_rank()),
m_primitive(create_NS_array(p, dec)),
m_conservative(create_NS_array(p, dec)),
m_bc_north(nullptr), m_bc_south(nullptr), //initialized below 
m_bc_east(nullptr), m_bc_west(nullptr),
m_bc_front(nullptr), m_bc_back(nullptr)
{

	create_boundary_conditions(p, dec);


}




void Domain::apply_boundary_conditions(Array4d& arr){
	
	m_bc_west->apply(arr);
	m_bc_east->apply(arr);

	
	m_bc_north->apply(arr);
	m_bc_south->apply(arr);

	
	m_bc_front->apply(arr);
	m_bc_back->apply(arr);
	
}




void Domain::create_boundary_conditions(const Params& p, const Decomposition& dec){

	if (dec.west_physical()){
		create_physical_west_boundary(p);
	}
	else { m_bc_west = new Boundary(); } //null boundary 


	if (dec.east_physical()){
		create_physical_east_boundary(p);
	}
	else { m_bc_east = new Boundary(); } //null boundary 

	if (dec.south_physical()){
		create_physical_south_boundary(p);
	}
	else { m_bc_south = new Boundary(); } //null boundary 

	if (dec.north_physical()){
		create_physical_north_boundary(p);
	}
	else { m_bc_north = new Boundary(); } //null boundary 


	if (dec.front_physical()){
		create_physical_front_boundary(p);
	}
	else { m_bc_front = new Boundary(); } //null boundary 


	if (dec.back_physical()){
		create_physical_back_boundary(p);
	}
	else { m_bc_back = new Boundary(); } //null boundary 

	
	
	


}




Array4d Domain::create_NS_array(const Params& p, const Decomposition& dec) const{

	//get the spatial indices based on the decomposition
	std::vector<int> node_count = dec.get_subdomain_node_count(p);

	size_t nx = node_count[0]; size_t ny = node_count[1];  size_t nz = node_count[2]; 
	size_t ngc = p.get_number_of_ghostcells();
	//size_t nq = p.dim + 1 + 1; //this is the number of conservative and primitive variables for N-S
	size_t nq = 5; // for now

	Array4d arr = Array4d(nx, ny, nz, nq, ngc);

	return arr;

}





void Domain::create_physical_west_boundary(const Params& p){

	

	auto search = p.boundary_conditions.find("West");
	
	if (search->second == "Neumann"){
		m_bc_west = new Neumann(search->second, 0, m_conservative);
	}

	//else if (search->second == "Inflow"){
	//	m_bc_west = new Inflow(p, m_points, search->second, 0, m_conservative);
	//}


}

void Domain::create_physical_east_boundary(const Params& p){

	auto search = p.boundary_conditions.find("East");

	if (search->second == "Neumann"){
		m_bc_east = new Neumann(search->second, 1, m_conservative);
	}

	

	

}

void Domain::create_physical_north_boundary(const Params& p){

	auto search = p.boundary_conditions.find("North");

	if (search->second == "Neumann"){
		m_bc_north = new Neumann(search->second, 2, m_conservative);
	}



}

void Domain::create_physical_south_boundary(const Params& p){

	auto search = p.boundary_conditions.find("South");

	if (search->second == "Neumann"){
		m_bc_south = new Neumann(search->second, 3, m_conservative);
	}

	

}

void Domain::create_physical_back_boundary(const Params& p){

	auto search = p.boundary_conditions.find("Back");

	if (search->second == "Neumann"){
		m_bc_back = new Neumann(search->second, 5, m_conservative);
	}

	

}

void Domain::create_physical_front_boundary(const Params& p){

	auto search = p.boundary_conditions.find("Front");

	if (search->second == "Neumann"){
		m_bc_front = new Neumann(search->second, 4, m_conservative);
	}

	

}






