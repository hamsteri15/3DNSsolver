#pragma once

#include "jada/extents.hpp"
#include "jada/neighbours.hpp"
#include "jada/rank.hpp"
#include "jada/utils.hpp"
#include <array>
#include <cstddef>

/// @brief Given coordinates of a subdomain returns the gird point offset wrt. position zero of the
/// global grid. The offset can be used to convert local indices to global indices by simple
/// addition: global = local + offset.
/// @param coords Coordinates of the local subdomain.
/// @param coord_dims The number of subdomains in each coordinate direction.
/// @param grid_dims The node count in the global grid
/// @return offset to the beginning index of the subdomain at 'coords'
static constexpr auto offset(auto coords, auto coord_dims, auto global_grid_dims) {

    runtime_assert(indices_in_bounds(coords, coord_dims), "Domain coordinates not in bounds.");
    using index_type = typename decltype(global_grid_dims)::value_type;

    auto offset(coords);
    for (size_t i = 0; i < rank(coords); ++i) {
        offset[i] = index_type(global_grid_dims[i] / coord_dims[i]) * coords[i];
    }
    return offset;
}

/// @brief Given coordinates of a subdomain returns the local grid point count in that subdomain. In
/// case of uneven splitting of the global_grid_dims to subdomains in a certain direction, the
/// reminding points are added to the last (largest coordinate) subdomain of that correspoding
/// direction. Example: global_grid_dims = {2,3}, coord_dims = {2,2}, the point count of the
/// subdomain at coords= {0,0} is {1,1} and the point count of the subdomain at coords = {0,1} is
/// {1,2}
/// @param coords Coordinates of the local subdomain.
/// @param coord_dims The number of subdomains in each coordinate direction.
/// @param grid_dims The node count in the global grid
/// @return the local dimensions of the subdomain at 'coords'
static constexpr auto local_dimensions(auto coords, auto coord_dims, auto global_grid_dims) {

    runtime_assert(indices_in_bounds(coords, coord_dims), "Domain coordinates not in bounds.");

    decltype(global_grid_dims) local_dims{};
    using index_type = typename decltype(local_dims)::value_type;

    for (size_t i = 0; i < rank(coords); ++i) {
        local_dims[i] = index_type(global_grid_dims[i] / coord_dims[i]);

        // Uneven points added to the last subdomain in the corresponding direction
        if (coords[i] == index_type(coord_dims[i] - 1)) {
            local_dims[i] += index_type(global_grid_dims[i] % coord_dims[i]);
        }
    }
    return local_dims;
}
