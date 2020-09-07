#include "reader.hpp"

void Reader::read_input_file(Params& p){

    //open the document
    //XMLDocument doc;
    //XMLDocument doc(COLLAPSE_WHITESPACE);

    bool processEntities=true;
    XMLDocument doc(processEntities, COLLAPSE_WHITESPACE);



    XMLError err = doc.LoadFile(m_infile_path);
    if (err != XML_SUCCESS) throw FileNotFoundError(m_infile_path);

    //read the root node from the document
    XMLNode* root = doc.FirstChildElement("Input");
    if (root == nullptr) throw InputFileError("Input (first element)");
    
   
    //read the inputs
    read_geometry(root, p);
    read_grid(root, p);
    read_fluid_properties(root, p);
    read_time(root, p);
    read_output_properties(root, p);
    read_initial_conditions(root, p);
    read_boundary_conditions(root, p);
    read_numerical_schemes(root, p);
    read_domain_decomposition(root, p);
    



}



void Reader::read_geometry(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("Geometry");
    if (element == nullptr) throw InputFileError("Geometry");


    read_parameter(element, "Dim", p.dim);
    read_parameter(element, "Lx", p.Lx);
    read_parameter(element, "Ly", p.Ly);
    read_parameter(element, "Lz", p.Lz);

}

void Reader::read_grid(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("Grid");
    if (element == nullptr) throw InputFileError("Grid");

    read_parameter(element, "Nx", p.nx);
    read_parameter(element, "Ny", p.ny);
    read_parameter(element, "Nz", p.nz);


}

void Reader::read_fluid_properties(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("FluidProperties");
    if (element == nullptr) throw InputFileError("FluidProperties");

    read_parameter(element, "Mu", p.mu);
    read_parameter(element, "Pr", p.Pr);
    read_parameter(element, "Gamma", p.gamma);
    
    

}

void Reader::read_time(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("Time");
    if (element == nullptr) throw InputFileError("Time");

    read_parameter(element, "EndT", p.end_t);
    read_parameter(element, "CFLmax", p.CFL_max);
    

}

void Reader::read_output_properties(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("OutputProperties");
    if (element == nullptr) throw InputFileError("OutputProperties");

    read_parameter(element, "OutputInterval", p.io_interval);
    read_parameter(element, "FileFormat", p.output_file_format);
    
}

void Reader::read_initial_conditions(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("InitialConditions");
    if (element == nullptr) throw InputFileError("InitialConditions");

    read_parameter(element, "Case", p.testcase);
       
}


void Reader::read_boundary_conditions(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("BoundaryConditions");
    if (element == nullptr) throw InputFileError("BoundaryConditions");

    std::string bc_west, bc_east, bc_north, bc_south, bc_front, bc_back;

    read_parameter(element, "West", bc_west);
    read_parameter(element, "East", bc_east);
    read_parameter(element, "North", bc_north);
    read_parameter(element, "South", bc_south);
    read_parameter(element, "Front", bc_front);
    read_parameter(element, "Back", bc_back);
    
    p.boundary_conditions.insert((std::make_pair("West", bc_west)));
    p.boundary_conditions.insert((std::make_pair("East", bc_east)));
    p.boundary_conditions.insert((std::make_pair("North", bc_north)));
    p.boundary_conditions.insert((std::make_pair("South", bc_south)));
    p.boundary_conditions.insert((std::make_pair("Front", bc_front)));
    p.boundary_conditions.insert((std::make_pair("Back", bc_back)));

    
       
}




void Reader::read_numerical_schemes(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("NumericalSchemes");
    if (element == nullptr) throw InputFileError("NumericalSchemes");

    read_convection_scheme(element, p);




    


    element = element->FirstChildElement("DiffusionScheme");
    if (element == nullptr) throw InputFileError("DiffusionScheme");

    read_parameter(element, "Scheme", p.diffusion_scheme);
    read_parameter(element, "Order", p.diffusion_scheme_order);
       
}

void Reader::read_convection_scheme(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("ConvectionScheme");
    if (element == nullptr) throw InputFileError("ConvectionScheme");


    read_parameter(element, "Scheme", p.convection_scheme);
    if(p.convection_scheme == "Weno"){
        read_parameter(element, "WeightType", p.weight_type);
        read_parameter(element, "RiemannSolver", p.riemann_solver);
    }

    //always
    read_parameter(element, "CharacteristicSpace", p.convection_in_characteristic_space);
    read_parameter(element, "Order", p.convection_scheme_order);

}


void Reader::read_domain_decomposition(XMLNode* root, Params& p){

    XMLElement* element = root->FirstChildElement("DomainDecomposition");
    if (element == nullptr) throw InputFileError("DomainDecomposition");

    read_parameter(element, "NprocX", p.nproc_x);
    read_parameter(element, "NprocY", p.nproc_y);
    read_parameter(element, "NprocZ", p.nproc_z);
    

       
}









void Reader::read_parameter(XMLNode* parent, const char* child_name, int& parameter){

    tinyxml2::XMLElement* child = parent->FirstChildElement(child_name);
    if (child == nullptr) throw InputFileError(child_name);

    child->QueryIntText(&parameter);
        
}

void Reader::read_parameter(XMLNode* parent, const char* child_name, double& parameter){

    tinyxml2::XMLElement* child = parent->FirstChildElement(child_name);
    if (child == nullptr) throw InputFileError(child_name);

    child->QueryDoubleText(&parameter);
}

void Reader::read_parameter(XMLNode* parent, const char* child_name, std::string& parameter){

    tinyxml2::XMLElement* child = parent->FirstChildElement(child_name);
    if (child == nullptr) throw InputFileError(child_name);

    parameter = child->GetText();

        
}


