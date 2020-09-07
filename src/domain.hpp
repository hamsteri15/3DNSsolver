#ifndef domain_hh
#define domain_hh

#include "communicator.hpp"

#include "params.hpp"

#include "./boundary_conditions/boundary.hpp"
#include "./boundary_conditions/boundary_neumann.hpp"
#include "./containers/array_4d.hpp"
#include "decomposition.hpp"
#include "grid.hpp"

class Domain {

public:
    Domain() = default;
    Domain(const Params& p, const Decomposition& dec);

    virtual ~Domain() {
        delete m_bc_north;
        delete m_bc_south;
        delete m_bc_west;
        delete m_bc_east;
        delete m_bc_front;
        delete m_bc_back;
    }

    ////////////////////////////////////////////////////////////
    ///
    ///    \brief 			Should not take parameters
    ///
    ////////////////////////////////////////////////////////////
    virtual void apply_boundary_conditions(Array4d& arr);

    Array4d&       get_NS_primitive_array() { return m_primitive; }
    const Array4d& get_NS_primitive_array() const { return m_primitive; }

    Array4d&       get_NS_conservative_array() { return m_conservative; }
    const Array4d& get_NS_conservative_array() const { return m_conservative; }

    int get_rank() const { return m_process_rank; }

    Array4d create_NS_array(const Params&, const Decomposition&) const;

private:
    int m_process_rank;

    // Primitive variables [i][j][k][rho, u, p, u, v, w]
    Array4d m_primitive;

    // Conservative variables [i][j][k][rho, rho*E, rho*u, rho*v, rho*w]
    Array4d m_conservative;

    // should be placed in an array
    Boundary* m_bc_north;
    Boundary* m_bc_south;
    Boundary* m_bc_west;
    Boundary* m_bc_east;
    Boundary* m_bc_front;
    Boundary* m_bc_back;

    ////////////////////////////////////////////////////////////
    ///
    /// 	\brief 			Creates boundary conditions for
    ///    					the subdomain
    ///
    ///		\param p		Global parameters
    ///
    ////////////////////////////////////////////////////////////
    virtual void create_boundary_conditions(const Params& p, const Decomposition& dec);
    void         create_physical_west_boundary(const Params& p);
    void         create_physical_east_boundary(const Params& p);
    void         create_physical_north_boundary(const Params& p);
    void         create_physical_south_boundary(const Params& p);
    void         create_physical_front_boundary(const Params& p);
    void         create_physical_back_boundary(const Params& p);
};

#endif
