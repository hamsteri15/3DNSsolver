#ifndef reader_hh
#define reader_hh

#include "error_messages.hpp"
#include "io/tinyxml2.hpp"
#include "params.hpp"

using namespace tinyxml2; // this is bad

class Reader {

public:
    Reader()  = default;
    ~Reader() = default;

    Reader(const char* infile_path)
        : m_infile_path(infile_path) {}

    void read_input_file(Params& p);

protected:
private:
    const char* m_infile_path;

    void read_geometry(XMLNode* root, Params& p);
    void read_grid(XMLNode* root, Params& p);
    void read_fluid_properties(XMLNode* root, Params& p);
    void read_time(XMLNode* root, Params& p);
    void read_output_properties(XMLNode* root, Params& p);
    void read_initial_conditions(XMLNode* root, Params& p);
    void read_boundary_conditions(XMLNode* root, Params& p);
    void read_numerical_schemes(XMLNode* root, Params& p);
    void read_convection_scheme(XMLNode* root, Params& p);
    void read_domain_decomposition(XMLNode* root, Params& p);

    void read_parameter(XMLNode* parent, const char* child_name, int& parameter);
    void read_parameter(XMLNode* parent, const char* child_name, double& parameter);
    void read_parameter(XMLNode* parent, const char* child_name, std::string& parameter);
};

#endif