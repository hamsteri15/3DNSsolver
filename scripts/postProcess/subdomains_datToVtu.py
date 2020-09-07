import numpy as np
import pylab as pl
from pyevtk.hl import pointsToVTK
from pyevtk.hl import gridToVTK 
import sys
import os
import glob as gb
import re

"""
USAGE:  
python datToVtu.py meshfile.dat resultfile.vtu
"""

results_dir = "./results/"

def main():

    
        
    
    mesh_files = gb.glob(results_dir + "*mesh*")
    
    
    times = get_times()
    time_count = len(times)
    
    subdomain_count = len(mesh_files)
    
    
    
    
    for i in range(subdomain_count):
        mesh_path = results_dir + "mesh_p_{}.dat".format(i)
        x,y,z = loadPoints(mesh_path) #point coordinates  
        #y = 1 - y #flip y axis

        xc, yc, zc = create_cells(x,y,z)
        nx = len(xc) - 1 
        ny = len(yc) - 1
        nz = len(zc) - 1
        
        ncells = nx*ny*nz





        for j in range(time_count):
                
            field_path = results_dir + "fields_{}_p_{}.dat".format(times[j],i)
            field_o_path = results_dir + "fields_p_{}_{}".format(i, times[j]) #points to vtu appends .vtu
            #rho, u, v, w, p, E = loadFields(field_path)
            rho, u, v, w, p = loadFields(field_path)
            #pointsToVTK("{}".format(field_o_path), x,y,z, data = {"rho":rho, "u":u, "v":v, "w":w ,"p":p, "E": E})



            rho = rho.reshape(nx, ny, nz)
            u = u.reshape(nx, ny, nz)
            v = v.reshape(nx, ny, nz)
            w = w.reshape(nx, ny, nz)
            p = p.reshape(nx, ny, nz)
            #E = E.reshape(nx, ny, nz)

            gridToVTK("{}".format(field_o_path), xc,yc,zc, cellData = {"rho":rho, "u":u, "v":v, "w":w ,"p":p})
        
def create_cells(x,y,z):
    
    #unique returns sorted array
    x_1d = np.unique(x)
    y_1d = np.unique(y)
    z_1d = np.unique(z)

    if (len(x_1d) > 1):
        dx = x_1d[1] - x_1d[0]
    else:
       dx = 0.001

    if (len(y_1d) > 1):
        dy = y_1d[1] - y_1d[0]
    else:
       dy = 0.001

    if (len(z_1d) > 1):
        dz = z_1d[1] - z_1d[0]
    else:
       dz = 0.001  

    

    #start of first cell face
    x_1d-=0.5*dx
    y_1d-=0.5*dy
    z_1d-=0.5*dz

    x_1d = np.append(x_1d, x_1d[-1] + dx)
    y_1d = np.append(y_1d, y_1d[-1] + dy)
    z_1d = np.append(z_1d, z_1d[-1] + dz)

    return x_1d, y_1d, z_1d

def get_times():


    field_files = gb.glob(results_dir + "*fields*")

    #'./results/fields_0_p_0.dat'

    times = []
    for f in field_files:
        
        if (".dat" in f):
            match = re.search('fields_(\d+)', f)
            times.append(match.group(1))
    return times    


def loadPoints(path):

    data = np.fromfile(path)
    x_idx = np.arange(0, len(data), 3)
    x_coords = data[x_idx]
    y_coords = data[x_idx + 1]
    z_coords = data[x_idx + 2]
    return x_coords, y_coords, z_coords
    
    
    
def loadFields(path):

    data = np.fromfile(path)
    
    rho_idx = np.arange(0, len(data), 5) #6
    #print rho_idx
    rho = data[rho_idx]
    p = data[rho_idx + 1]
    u = data[rho_idx + 2]
    v = data[rho_idx + 3]
    w = data[rho_idx + 4]
    #E = data[rho_idx + 5]
    
    #return rho,u,v,w,p,E
    return rho,u,v,w,p


main()
