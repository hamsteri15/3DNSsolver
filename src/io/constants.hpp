#pragma once

#include <string>

struct Constants {

    inline static const std::string checkpoint_extension = "checkpoint_";

    inline static const std::string grid_base_path = "grid/";

    inline static const std::string grid_edge_path = grid_base_path + std::string("edges/");

    inline static const std::string grid_xdmf_info_path = grid_base_path + std::string("xdmf/");
};