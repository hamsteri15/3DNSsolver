#ifndef testcases_hh
#define testcases_hh



#include <cstdlib>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include "params.hpp"
#include "domain.hpp"
#include "grid.hpp"
#include "containers/array_4d.hpp"

namespace testcases
{

	void set_quarters_xy(Domain& domain, const Grid& grid, const Params& p,
					std::vector<double> NW, std::vector<double> NE, 
					std::vector<double> SW, std::vector<double> SE);

	void set_quarters_yz(Domain& domain, const Grid& grid, const Params& p,
					std::vector<double> NW, std::vector<double> NE, 
					std::vector<double> SW, std::vector<double> SE);

	void rank(Domain& domain, const Grid& grid, const Params& p);


	void case3(Domain& domain, const Grid& grid, const Params& p);
	void case4(Domain& domain, const Grid& grid, const Params& p);
	void case6(Domain& domain, const Grid& grid, const Params& p);
	void case12(Domain& domain, const Grid& grid, const Params& p);
	void case15(Domain& domain, const Grid& grid, const Params& p);
	void case17(Domain& domain, const Grid& grid, const Params& p);
	void sod_xdir(Domain& domain, const Grid& grid, const Params& p);
	void sod_ydir(Domain& domain, const Grid& grid, const Params& p);
	void sod_zdir(Domain& domain, const Grid& grid, const Params& p);
	void shear_layer_xy(Domain& domain, const Grid& grid, const Params& p);
	void shear_layer_3d(Domain& domain, const Grid& grid, const Params& p);

}

#endif








