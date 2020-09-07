import xml.etree.cElementTree as ET #part of standard library :)
import xml.dom.minidom #for printing pretty files
import h5py
import re
import numpy as np
from io import BytesIO
import argparse
import ntpath

OFILE_WRITE_PRECISION = "4"


                 
def main():
    
    
    parser = argparse.ArgumentParser()
    parser.add_argument("input", help="input file path", action="store")
                   
    args = parser.parse_args()
    
     
    
    i_fpath = args.input #full path to input file
    
    base_path, i_fname, o_fname = parse_input_string(i_fpath)
    
    
    

    root = ET.Element("Xdmf")
    Domain = ET.SubElement(root, "Domain")
    Grid = ET.SubElement(Domain, "Grid", Name="Region", GridType="Collection", CollectionType="Temporal")
    
    checkpoints, times = get_checkpoints_and_times(i_fpath)
    
    nx, ny, nz = get_grid_dimensions(i_fpath)
    
    for i in range(len(checkpoints)):
    
        write_checkpoint(Grid, checkpoints[i], times[i], i_fname, nx, ny, nz)
    

    

    
    tree = ET.ElementTree(root)
    #print_pretty(tree)
        
    write_pretty(tree, base_path + o_fname)
    

    
def parse_input_string(full_path):

    i_fname = ntpath.basename(full_path) #extract the filename from path
    base_path = full_path.replace(i_fname, "")
    o_fname = i_fname.replace(".h5", ".xdmf")
    
    
    return base_path, i_fname, o_fname



def write_checkpoint(mainGrid, checkpoint, time, i_fname, nx, ny, nz):



    Grid = ET.SubElement(mainGrid, "Grid", GridType="Uniform", Name="Mesh")
    ET.SubElement(Grid, "Time", Type="Single", Value=str(time))
        
    set_mesh_info(Grid, i_fname, nx, ny, nz)
    set_fields(Grid, checkpoint, i_fname, nx, ny, nz)
    


    

def set_fields(Grid, groupname, i_fname, nx, ny, nz):
    
    fields = ["rho", "p", "u", "v", "w"]
   

    #this is so annoying, in the attributes section
    #3d arrays are not supported with length 1 in some direction
    #in the mesh section there's no problem
    if nz == 1:
        dims_string="{} {}".format(nx,ny)
    else:
        dims_string="{} {} {}".format(nx,ny,nz)



    for field in fields:
        
        field_path = (i_fname + ":/" + groupname + "/" + field).encode('utf8')
        
        Attribute = ET.SubElement(Grid, "Attribute", Name=field, AttributeType="Scalar", Center="Node")
        DataItem = ET.SubElement(Attribute, "DataItem", \
                                            Dimensions=dims_string, \
                                            NumberType="Float", \
                                            Precision=OFILE_WRITE_PRECISION,\
                                            Format="HDF").text=field_path
    
    

def set_mesh_info(Grid, i_fname, nx, ny, nz):
    """
    Sets the mesh information of the given .h5 file under the Grid node
    in the infile, the points are a 4-dimensional array with
    \coords[nx,ny,nz,(x,y,z)]
    
    """


    #topology node
    dims_string="{} {} {}".format(nx,ny,nz) #dimensions of the array containing the points
    Topo = ET.SubElement(Grid, "Topology", TopologyType="3DSMesh", Dimensions=dims_string)


    fields = ["x_coords", "y_coords", "z_coords"]
    

    #geometry node
    Geom = ET.SubElement(Grid, "Geometry", GeometryType="X_Y_Z")

    
    for field in fields:
        
    
        infile_array_path = (i_fname + ":/mesh/{}".format(field)).encode('utf8')


        DataItem = ET.SubElement(Geom, "DataItem", \
                            Name="coords", \
                            Dimensions=dims_string, \
                            NumberType="Float", \
                            Precision=OFILE_WRITE_PRECISION, \
                            Format="HDF").text=infile_array_path

    
    



def get_checkpoints_and_times(infile_path):
    """
    Reads the groups labeled as 'checkpoint_n'
    from the input file and a single double
    dataset 'time' from the corresponding 
    group
    """
    
    f = h5py.File(infile_path, 'r')
    
    keys = f.keys()
    
    times = []
    checkpoints = []
    for key in keys:
        if "checkpoint" in key:
            time = f["{}/time".format(key)][0]
            times.append(time)
            checkpoints.append(key)
            


    times = np.array(times)
    sorted_indices = np.argsort(times)
    times = times[sorted_indices]
    #checkpoints = checkpoints[sorted_indices]

    checkpoints = []
    for i in range(len(times)):
        checkpoints.append("checkpoint_{0}".format(i))



    #checkpoints = ["checkpoint_0"]
    #times = [0.]

    return checkpoints, times
    
    

def get_grid_dimensions(infile_path):
    """
    Reads the grid dimensions from the file
    from a group 'mesh' which must contain
    dataset 'x_coords'
    """
        
    f = h5py.File(infile_path, 'r')

    coords = f['/mesh/x_coords']

    nx = coords.shape[0]
    ny = coords.shape[1]
    nz = coords.shape[2]


    return nx, ny, nz






def get_header():
    
    header = '<?xml version="1.0" ?>\n<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>'.encode('utf8')
    return header





def write_pretty(tree, o_file):
    
    
    string = get_pretty_string(tree)
    f = open(o_file, "w")
    f.write(string)



def get_pretty_string(tree):
    """
    writes the input tree with newlines and tabs
    header written manually
    """ 
    
    string = tree_to_string(tree)

    dom = xml.dom.minidom.parseString(string)
    #this appends the xmlversion to the first line, 
    #that line is deleted below because its in the header already
    pretty_string = dom.toprettyxml()
    pretty_string = pretty_string.split("\n",1)[1];

    header = get_header()

    pretty_string = header + pretty_string
    return pretty_string    



def tree_to_string(tree):
    """
    dirty hack to _not_ write the header. The header is written manually
    ET.tostring does not have xml_declaration parameter...
    """
    #write to imaginary file and get the string from there
    f = BytesIO()
    tree.write(f, encoding='utf-8', xml_declaration=False) 
    string = f.getvalue() 
                    
    return string

def print_pretty(tree):

    print get_pretty_string(tree)

    


def print_object_methods(obj):
    """
    useful for checking what methods can be called from an arbitrary class
    """ 

    object_methods = [method_name for method_name in dir(obj)
                  if callable(getattr(obj, method_name))]
    
    
    print object_methods



















main()
