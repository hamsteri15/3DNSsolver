#pragma once

#include <string>

struct Constants {

    inline static const std::string checkpoint_path = "checkpoints/";

    inline static const std::string checkpoint_extension = "checkpoint_";

    inline static const std::string grid_base_path = "grid/";

    inline static const std::string grid_vertex_path = grid_base_path + std::string("vertices/");
    
    inline static const std::string vertex_extension = "vertices_";

    inline static const std::string grid_xdmf_info_path = grid_base_path + std::string("xdmf/");
    
    inline static const std::string xdmf_topology_name = "topology";

    inline static const std::string xdmf_geometry_name = "geometry";
    
};