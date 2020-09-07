import numpy as np
import pylab as pl
from pyevtk.hl import pointsToVTK
import sys
import os

"""
USAGE:	
python datToVtu.py meshfile.dat resultfile.vtu
"""


def main():

	
	results_dir = "./results/"	
	
	
	

	"""
	mesh_path = sys.argv[-3]
	
	field_path = sys.argv[-2]
	
	o_path = sys.argv[-1]
	
	#remove possible file extensions since pointsToVTK already appends .vtu
	o_path = ('.').join(o_path.split('.')[:-1])
	
	x,y,z = loadMesh(mesh_path)

	
	y = 1 - y #flip y axis
	
	
	rho, u, v, w, p, E = loadFields(field_path)
	
	
	pointsToVTK("{}".format(o_path), x,y,z, data = {"rho":rho, "u":u, "v":v, "w":w ,"p":p, "E": E})
	"""
	

def loadMesh(path):

	data = np.fromfile(path)
	x_idx = np.arange(0, len(data), 3)
	x_coords = data[x_idx]
	y_coords = data[x_idx + 1]
	z_coords = data[x_idx + 2]
	return x_coords, y_coords, z_coords
	
	
	
def loadFields(path):

	data = np.fromfile(path)
	
	rho_idx = np.arange(0, len(data), 6)
	#print rho_idx
	rho = data[rho_idx]
	p = data[rho_idx + 1]
	u = data[rho_idx + 2]
	v = data[rho_idx + 3]
	w = data[rho_idx + 4]
	E = data[rho_idx + 5]
	
	return rho,u,v,w,p,E


main()
