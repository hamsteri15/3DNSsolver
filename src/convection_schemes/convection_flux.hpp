#ifndef ConvectionFlux_hh
#define ConvectionFlux_hh
#include <cmath> //sqrt
#include <cstring> //std::memcpy
#include <algorithm> //std::max
#include <vector>

#include "containers/array_3d.hpp"
#include "containers/array_4d.hpp"


class ConvectionFlux {

	

		
	public:
	
		ConvectionFlux() = default;

        ~ConvectionFlux() = default;

        ////////////////////////////////////////////////////////////
		///
		///    	\brief			Computes the convection flux for Euler equations
		///		
		///
		///		\param (input)			W vector of primitive variables (rho, p,     u,     v,	  w)
		///		\param (output)			F flux (rho*q_n, rho*q_n*H, rho*q_n*u + p*nx, rho*q_n*v + p*ny, rho*q_n*w + p*nz)
        ///     \param (input)          gamma ratio of specific heats
        ///     \param (input)          normal_x, normal_y, normal_z normal vectors in cartesian directions
		////////////////////////////////////////////////////////////
		void euler_flux_from_primitive(const double* W, double* F, double gamma, double normal_x, double normal_y, double normal_z);
		
		
		
		////////////////////////////////////////////////////////////
		///
		///    	\brief		Computes the left (L) and right (R) eigenvector matrices for 3D-Euler system.
		///					The syntax here corresponds to eq. 3.6.11 and 3.6.12 in IdoLikeCFD. Note the row
		///					shift in L and column shift in R to match the flux vector reordering. 
		///		
		///
		///		\param (input)	W vector of primitive variables (rho, p,     u,     v,	  w) typically an avarage at W_{i + 1/2}				
		///     \param (input)          gamma ratio of specific heats
		///     \param (n)              unit normal vector
		///		\param (l,m)			mutually orthogonal unit vectors to n (see the function below)
		///     \param (output)         L left eigenvectors
        ///     \param (output)         R right eigenvectors
		////////////////////////////////////////////////////////////
		void eigenvectors(double rho, double p, double u, double v, double w, double gamma, 
						  std::vector<double> n, std::vector<double> l, std::vector<double> m,
						  double* L, double* R ) const;


		////////////////////////////////////////////////////////////
		///
		///    	\brief		Given a unit normal, this function computes,
		///					mutually orthogonal vectors to the unit normal,
		///					such that zero division is avoided when constructing the 
		///					eigenvectors.
		///     \param      unit normal vector
		////////////////////////////////////////////////////////////
		std::vector<double> orthogonal_unit_vectors(std::vector<double> unit_normal) const;

		////////////////////////////////////////////////////////////
		///
		///    	\brief		Computes the eigenvalues eig = [lam1, lam2, lam3, lam4, lam5] for the 3D-Euler system
		///		
		///
		///		\param (input)					W vector of primitive variables (rho, p,     u,     v,	  w) 						
		///		\param (output)					eig eigenvalues
		///     \param (input)          		gamma ratio of specific heats
        ///     \param (input) nx,ny,nz         components of the cartesian unit normal 
		///										to which direction to project the eigenvalue
		////////////////////////////////////////////////////////////
		void eigenvalues(const double* W, double* eig, double gamma, double nx, double ny, double nz);


		////////////////////////////////////////////////////////////
		///
		///    	\brief		Computes the maximum eigenvalue
		///		
		///
		///		\param (input)					W vector of primitive variables (rho, p,     u,     v,	  w) 								
		///     \param (input)          		gamma ratio of specific heats
        ///     \param (input) nx,ny,nz         components of the cartesian unit normal 
		///										to which direction to project the eigenvalue
		////////////////////////////////////////////////////////////
		double max_eigenvalue(const double* W, double gamma, double nx, double ny, double nz);

		

		Array4d form_flux(const Array4d& primitive, double gamma, double normal_x, double normal_y, double normal_z) const;

		Array3d max_eigenvalue(const Array4d& primitive, double gamma, double normal_x, double normal_y, double normal_z) const;

		


	private:

	

    
};



#endif
