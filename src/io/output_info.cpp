#include "output_info.hpp"


OutputInfo::OutputInfo(int write_interval, std::string type, std::string outfile_path) :
m_write_interval(write_interval), 
m_type(type),
m_outfile_path(outfile_path)
{
   

    m_number_of_checkpoints = 0;
    m_grid_written = 0;
    m_current_iteration = 0;
    m_current_runtime = 0.0;

}


int OutputInfo::is_write_time() const{


    if (m_current_iteration % m_write_interval == 0){
        return 1;
    }
    return 0;


}

std::string OutputInfo::get_current_checkpoint() const{

    std::string ret;

    ret = "checkpoint_" + std::to_string(m_number_of_checkpoints);
    
    return ret;

}

void OutputInfo::print() const{

    std::cout << "Writing " << m_type << std::endl;


}