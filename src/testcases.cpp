#include "testcases.hpp"

namespace testcases
{

void sod_xdir(Domain& domain, const Grid& grid, const Params& p){

	double pl = 1.;   double pr = 0.1;
	double rhol = 1.; double rhor = 0.125;

	std::vector<double> NW = {rhol, pl, 0.0, 0.0, 0.0};
	std::vector<double> NE = {rhor, pr, 0.0, 0.0, 0.0};
	std::vector<double> SW = {rhol, pl, 0.0, 0.0, 0.0};
	std::vector<double> SE = {rhor, pr, 0.0, 0.0, 0.0};


	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void sod_ydir(Domain& domain, const Grid& grid, const Params& p){

	double pl = 1.;   double pr = 0.1;
	double rhol = 1.; double rhor = 0.125;

	std::vector<double> NW = {rhol, pl, 0.0, 0.0, 0.0};
	std::vector<double> NE = {rhol, pl, 0.0, 0.0, 0.0};
	std::vector<double> SW = {rhor, pr, 0.0, 0.0, 0.0};
	std::vector<double> SE = {rhor, pr, 0.0, 0.0, 0.0};


	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void sod_zdir(Domain& domain, const Grid& grid, const Params& p){

	double pl = 1.;   double pr = 0.1;
	double rhol = 1.; double rhor = 0.125;

	std::vector<double> NW = {rhol, pl, 0.0, 0.0, 0.0};
	std::vector<double> NE = {rhor, pr, 0.0, 0.0, 0.0};
	std::vector<double> SW = {rhol, pl, 0.0, 0.0, 0.0};
	std::vector<double> SE = {rhor, pr, 0.0, 0.0, 0.0};


	set_quarters_yz(domain, grid, p, NW, NE, SW, SE);
}


void rank(Domain& domain, const Grid& grid, const Params& p){

	double rank = domain.get_rank() * 1.0;
	std::vector<double> NW = {rank, rank, rank, rank, rank};
	std::vector<double> NE = {rank, rank, rank, rank, rank};
	std::vector<double> SW = {rank, rank, rank, rank, rank};
	std::vector<double> SE = {rank, rank, rank, rank, rank};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);

}

void case3(Domain& domain, const Grid& grid, const Params& p){

	std::vector<double> NW = {0.5323, 0.3, 1.206, 0.0, 0.0};
	std::vector<double> NE = {1.5, 1.5, 0.0, 0.0, 0.0};
	std::vector<double> SW = {0.138, 0.029, 1.206, -1.206, 0.0};
	std::vector<double> SE = {0.5323, 0.3, 0.0, -1.206, 0.0};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void case4(Domain& domain, const Grid& grid, const Params& p){

	std::vector<double> NW = {0.5065, 0.35, 0.8939, 0.0, 0.0};
	std::vector<double> NE = {1.1, 1.1, 0.0, 0.0, 0.0 };
	std::vector<double> SW = {1.1, 1.1, 0.8939, -0.8938, 0.0};
	std::vector<double> SE = {0.5065, 0.35, 0.0, -0.8939, 0.0};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void case6(Domain& domain, const Grid& grid, const Params& p){

	std::vector<double> NW = {2.0, 1.0, 0.75, -0.5, 0.0};
	std::vector<double> NE = {1.0, 1.0, 0.75, 0.5, 0.0};
	std::vector<double> SW = {1.0, 1.0, -0.75, -0.5, 0.0};
	std::vector<double> SE = {3.0, 1.0, -0.75, 0.5, 0.0};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void case12(Domain& domain, const Grid& grid, const Params& p){

	std::vector<double> NW = {1.0, 1.0, 0.7276, 0.0, 0.0};
	std::vector<double> NE = {0.5313, 0.4, 0.0, 0.0, 0.0};
	std::vector<double> SW = {0.8, 1.0, 0.0, 0.0, 0.0};
	std::vector<double> SE = {1.0, 1.0, 0.0, -0.7276, 0.0};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void case15(Domain& domain, const Grid& grid, const Params& p){

	std::vector<double> NW = {0.5197, 0.4, -0.6259, 0.3, 0.0};
	std::vector<double> NE = {1.0, 1.0, 0.1, 0.3, 0.0};
	std::vector<double> SW = {0.8, 0.4, 0.1, 0.3, 0.0};
	std::vector<double> SE = {0.5313, 0.4, 0.1, -0.4276, 0.0};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}

void case17(Domain& domain, const Grid& grid, const Params& p){

	std::vector<double> NW = {2.0, 1.0, 0.0, 0.3, 0.0};
	std::vector<double> NE = {1.0, 1.0, 0.0, 0.4, 0.0};
	std::vector<double> SW = {1.0625, 0.4, 0.0, -0.2145};
	std::vector<double> SE = {0.5197, 0.4, 0.0, 1.1259, 0.0};
	set_quarters_xy(domain, grid, p, NW, NE, SW, SE);
}




void set_quarters_xy(Domain& domain, const Grid& grid, const Params& p,
					std::vector<double> NW, std::vector<double> NE, 
					std::vector<double> SW, std::vector<double> SE)
{

	const Array4d& coords = grid.get_coords();
	Array4d& W = domain.get_NS_primitive_array();

	double Lx = p.Lx;
	double Ly = p.Ly;	

	for (size_t i = coords.i_begin(); i < coords.i_end(); i++){
	for (size_t j = coords.j_begin(); j < coords.j_end(); j++){
	for (size_t k = coords.k_begin(); k < coords.k_end(); k++){
		
		
		
		double x = coords(i,j,k,0);
		double y = coords(i,j,k,1);

		
		//North-West quadrant
		if (x <= Lx/2. && y <= Ly/2. ){
			
			W(i,j,k,0) = NW[0]; //rho
			W(i,j,k,1) = NW[1]; //p
			W(i,j,k,2) = NW[2]; //u
			W(i,j,k,3) = NW[3]; //v
			W(i,j,k,4) = NW[4]; //w

			//std::cout << W(i,j,k,0) << std::endl;

		}
		//North-East quadrant
		else if (x > Lx/2. && y <= Ly/2.){	
			
			W(i,j,k,0) = NE[0]; //rho
			W(i,j,k,1) = NE[1]; //p
			W(i,j,k,2) = NE[2]; //u
			W(i,j,k,3) = NE[3]; //v
			W(i,j,k,4) = NE[4]; //w
			
			
		}

		//South-West quadrant
		else if (x <= Lx/2. && y > Ly/2. ){
			
			W(i,j,k,0) = SW[0]; //rho
			W(i,j,k,1) = SW[1]; //p
			W(i,j,k,2) = SW[2]; //u
			W(i,j,k,3) = SW[3]; //v
			W(i,j,k,4) = SW[4]; //w
			

		}
		//South-East quadrant
		else {
		
			W(i,j,k,0) = SE[0]; //rho
			W(i,j,k,1) = SE[1]; //p
			W(i,j,k,2) = SE[2]; //u
			W(i,j,k,3) = SE[3]; //v
			W(i,j,k,4) = SE[4]; //w

		}
				
			
	}}}




}

void set_quarters_yz(Domain& domain, const Grid& grid, const Params& p,
					std::vector<double> NW, std::vector<double> NE, 
					std::vector<double> SW, std::vector<double> SE)
{

	const Array4d& coords = grid.get_coords();
	Array4d& W = domain.get_NS_primitive_array();

	double Lz = p.Lz;
	double Ly = p.Ly;	

	for (size_t i = coords.i_begin(); i < coords.i_end(); i++){
	for (size_t j = coords.j_begin(); j < coords.j_end(); j++){
	for (size_t k = coords.k_begin(); k < coords.k_end(); k++){
		
		
		
		double z = coords(i,j,k,2);
		double y = coords(i,j,k,1);

		
		//North-West quadrant
		if (z <= Lz/2. && y <= Ly/2. ){
			
			W(i,j,k,0) = NW[0]; //rho
			W(i,j,k,1) = NW[1]; //p
			W(i,j,k,2) = NW[2]; //u
			W(i,j,k,3) = NW[3]; //v
			W(i,j,k,4) = NW[4]; //w

			//std::cout << W(i,j,k,0) << std::endl;

		}
		//North-East quadrant
		else if (z > Lz/2. && y <= Ly/2.){	
			
			W(i,j,k,0) = NE[0]; //rho
			W(i,j,k,1) = NE[1]; //p
			W(i,j,k,2) = NE[2]; //u
			W(i,j,k,3) = NE[3]; //v
			W(i,j,k,4) = NE[4]; //w
			
			
		}

		//South-West quadrant
		else if (z <= Lz/2. && y > Ly/2. ){
			
			W(i,j,k,0) = SW[0]; //rho
			W(i,j,k,1) = SW[1]; //p
			W(i,j,k,2) = SW[2]; //u
			W(i,j,k,3) = SW[3]; //v
			W(i,j,k,4) = SW[4]; //w
			

		}
		//South-East quadrant
		else {
		
			W(i,j,k,0) = SE[0]; //rho
			W(i,j,k,1) = SE[1]; //p
			W(i,j,k,2) = SE[2]; //u
			W(i,j,k,3) = SE[3]; //v
			W(i,j,k,4) = SE[4]; //w

		}
				
			
	}}}




}



void shear_layer_xy(Domain& domain, const Grid& grid, const Params& p){

	const Array4d& coords = grid.get_coords();
	Array4d& W = domain.get_NS_primitive_array();

	double Lx = p.Lx;
	double Ly = p.Ly;	
	double y_center = 0.5*Ly;
	double x_center = 0.5*Lx;
	double stripe_w = 0.1*Ly;
	double pi = 3.14159265358979323846;
	for (size_t i = coords.i_begin(); i < coords.i_end(); i++){
	for (size_t j = coords.j_begin(); j < coords.j_end(); j++){
	for (size_t k = coords.k_begin(); k < coords.k_end(); k++){
		
		double x = coords(i,j,k,0) - x_center;
		double y = coords(i,j,k,1) - y_center;

		//random number between 0 and 1
		//double z_pertb = 0.1*((double) rand() / (RAND_MAX));
		double y_pertb = 0.01*((double) rand() / (RAND_MAX));

		double z_pertb = 0.0;
		//double y_pertb = 0.0;

		if (std::abs(y) <= stripe_w){
			

				W(i,j,k,0)= 1.; //rho THIS SHOULD BE TWO
				W(i,j,k,1) = 2.5; //p
				W(i,j,k,2) = -0.5; //u
				W(i,j,k,3)  = y_pertb; //*sin(2.0*pi*x/p.Lx); //v
				W(i,j,k,4)  = z_pertb; //w 


			
		}
			
		else{
			

				W(i,j,k,0)= 1; //rho
				W(i,j,k,1) = 2.5; //p
				W(i,j,k,2) = 0.5; //u
				W(i,j,k,3)  = 0.0*sin(2.0*pi*x/p.Lx); //v
				W(i,j,k,4)  = z_pertb; //w 	
			
		}


	}}}	


}


void shear_layer_3d(Domain& domain, const Grid& grid, const Params& p){

	const Array4d& coords = grid.get_coords();
	Array4d& W = domain.get_NS_primitive_array();

	double Lx = p.Lx;
	double Ly = p.Ly;	
	double y_center = 0.5*Ly;
	double x_center = 0.5*Lx;
	double stripe_w = 0.25*Ly;
	double pi = 3.14159265358979323846;
	for (size_t i = coords.i_begin(); i < coords.i_end(); i++){
	for (size_t j = coords.j_begin(); j < coords.j_end(); j++){
	for (size_t k = coords.k_begin(); k < coords.k_end(); k++){
		
		double x = coords(i,j,k,0) - x_center;
		double y = coords(i,j,k,1) - y_center;

		//random number between 0 and 1
		double z_pertb = 0.02*((double) rand() / (RAND_MAX));
		double y_pertb = 0.02*((double) rand() / (RAND_MAX));

		//double z_pertb = 0.0;
		//double y_pertb = 0.0;

		if (std::abs(y) <= stripe_w){
			

				W(i,j,k,0)= 2.; //rho
				W(i,j,k,1) = 2.5; //p
				W(i,j,k,2) = -0.5; //u
				W(i,j,k,3)  = 0.01*sin(2.0*pi*x/p.Lx) + y_pertb; //v
				W(i,j,k,4)  = z_pertb; //w 


			
		}
			
		else{
			

				W(i,j,k,0)= 1; //rho
				W(i,j,k,1) = 2.5; //p
				W(i,j,k,2) = 0.5; //u
				W(i,j,k,3)  = 0.01*sin(2.0*pi*x/p.Lx); //v
				W(i,j,k,4)  = z_pertb; //w 	
			
		}


	}}}	


}



}//namespace

