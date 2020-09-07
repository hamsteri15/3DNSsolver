import numpy as np




def main():
    
    gamma = 1.4

    nx = 1.0
    ny = 0.0
    nz = 0.0

    lx, ly, lz, mx, my, mz = others(nx,ny,nz)

    rho = 1.0
    p = 1.2
    u = 0.1
    v = 0.3
    w = 0.54
    
    row_switch = False

    R = right(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz)
    L = left(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz)
    eigvals = eig(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz)
    Jac_analytic = jacobian(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz) #this is the analytic jacobian
    
    print "no row switch R*L"
    print np.round(R*L,4)


    row_switch = True

    R = right(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz)
    L = left(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz)
    eigvals = eig(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz)
    Jac_analytic = jacobian(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz) #this is the analytic jacobian
    
    print "with row switch R*L"
    print np.round(R*L,4)

   


def others(nx, ny, nz):

    tempx = ny*ny + nz*nz
    tempy = nz*nz + nx*nx
    tempz = nx*nx + ny*ny
    
    if     ( tempx >= tempy and tempx >= tempz ):
       lx =  0.0
       ly = -nz
       lz =  ny
    elif ( tempy >= tempx and tempy >= tempz ):
       lx = -nz
       ly =  0.0
       lz =  nx
    elif ( tempz >= tempx and tempz >= tempy ):
       lx = -ny
       ly =  nx
       lz =  0.0

    #ensure that l's are unit vectors
    temp = np.sqrt(lx*lx + ly*ly + lz*lz)
    lx = lx/temp
    ly = ly/temp
    lz = lz/temp
    

    mx = ny*lz - nz*ly
    my = nz*lx - nx*lz
    mz = nx*ly - ny*lx

    abs_n_cross_l = np.sqrt(mx**2 + my**2 + mz**2)
    mx = mx / abs_n_cross_l
    my = my / abs_n_cross_l
    mz = mz / abs_n_cross_l


    return lx, ly, lz, mx, my, mz


def jacobian(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz):

    """
    Gives the analytic Jacobian without row switch
    """
    
     

    c2 = gamma/(p*rho)  
    c = np.sqrt(c2)

    qn = u*nx + v*ny + w*nz
    ql = u*lx + v*ly + w*lz
    qm = u*mx + v*my + w*mz


    q2 = u*u + v*v + w*w

    K = gamma - 1.0
    K_pc = K / (2.0*c2) 

    H = c2 / (gamma - 1.0) + 0.5 * q2
        
    
    J1 = [0.0,      nx,     ny,     nz,     0.0]
    J2 = [0.5*K * q2 *nx - u*qn,        u*nx - K*u*nx + qn,     u*ny - K*v*nx,          u*nz - K*w*nx,          K*nx]
    J3 = [0.5*K * q2 *ny - v*qn,        v*nx - K*u*ny,          v*ny - K*v*ny + qn,     v*nz - K*w*ny,          K*ny]
    J4 = [0.5*K * q2 *nz - w*qn,        w*nx - K*u*nz,          w*ny - K*v*nz,          w*nz - K*w*nz + qn,     K*nz]
    J5 = [(0.5*K*q2 - H)*qn,            H*nx - K*u*qn,          H*ny - K*v*qn,          H*nz - K*w*qn,          gamma*qn]
    
    J = np.matrix([J1, J2, J3, J4, J5])

    return J
    





def right(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz):

      


    c2 = gamma/(p*rho)  
    c = np.sqrt(c2)

    qn = u*nx + v*ny + w*nz
    ql = u*lx + v*ly + w*lz
    qm = u*mx + v*my + w*mz


    q2 = u*u + v*v + w*w
    H = c2 / (gamma - 1.0) + 0.5 * q2

    
  
    R1 = [1.0, 1.0, 1.0, 0.0, 0.0]
    R2 = [u - c*nx, u, u + c*nx, lx, mx]
    R3 = [v - c*ny, v, v + c*ny, ly, my]
    R4 = [w - c*nz, w, w + c*nz, lz, mz]
    R5 = [H - qn*c, 0.5*q2, H + qn*c, ql, qm]

    RR = np.matrix([R1, R2, R3, R4, R5])

    if (row_switch):
      
        #Here the columns are switched
        R1 = [1.0, 0.0, 1.0, 1.0, 0.0]
        R2 = [u - c*nx, mx, u, u + c*nx, lx]
        R3 = [v - c*ny, my, v, v + c*ny, ly]
        R4 = [w - c*nz, mz, w, w + c*nz, lz]
        R5 = [H - qn*c, qm, 0.5*q2, H + qn*c, ql]


        R = np.matrix([R1, R2, R3, R4, R5])
        

    else:
        R = RR
    

    return R



def left(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz):

    c2 = gamma/(p*rho)  
    c = np.sqrt(c2)

    qn = u*nx + v*ny + w*nz
    ql = u*lx + v*ly + w*lz
    qm = u*mx + v*my + w*mz

    q2 = u*u + v*v + w*w

    K = gamma - 1.0
    K_pc = K / (2.0*c2) 

    #org
    L1 = [0.5*K_pc*q2 + qn/(2.0*c), -(K_pc*u + nx/(2.0*c)), -(K_pc*v + ny/(2.0*c)), -(K_pc*w + nz/(2.0*c)), K_pc]
    L2 = [1.0 - K_pc*q2,            K*u/c2,                 K*v/c2,                 K*w/c2,                -K/c2]
    L3 = [0.5*K_pc*q2 - qn/(2.0*c), -(K_pc*u - nx/(2.0*c)), -(K_pc*v - ny/(2.0*c)), -(K_pc*w - nz/(2.0*c)), K_pc]
    L4 = [-ql,                      lx,                     ly,                     lz,                     0.0 ]
    L5 = [-qm,                      mx,                     my,                     mz,                     0.0]
   


    LL = np.matrix([L1, L2, L3, L4, L5])


    if (row_switch):
        
        L1 = [0.5*K_pc*q2 + qn/(2.0*c), -(K_pc*u + nx/(2.0*c)), -(K_pc*v + ny/(2.0*c)), -(K_pc*w + nz/(2.0*c)), K_pc]
        L2 = [-qm,                      mx,                     my,                     mz,                     0.0]
        L3 = [1.0 - K_pc*q2,            K*u/c2,                 K*v/c2,                 K*w/c2,                -K/c2]
        L4 = [0.5*K_pc*q2 - qn/(2.0*c), -(K_pc*u - nx/(2.0*c)), -(K_pc*v - ny/(2.0*c)), -(K_pc*w - nz/(2.0*c)), K_pc]
        L5 = [-ql,                      lx,                     ly,                     lz,                     0.0 ]
        
    
        L = np.matrix([L1, L2, L3, L4, L5])
        

        """    
        #Mara python
        L = np.matrix(np.zeros((5,5)))
        L[0,:] = LL[1,:] 
        L[1,:] = LL[2,:] 
        L[2,:] = LL[0,:]
        L[3,:] = LL[3,:] 
        L[4,:] = LL[4,:]
        """

        """
        #Own
        L = np.matrix(np.zeros((5,5)))
        L[0,:] = LL[0,:] 
        L[1,:] = LL[4,:] 
        L[2,:] = LL[1,:]
        L[3,:] = LL[2,:] 
        L[4,:] = LL[3,:]
        """

    else:
        L = LL


    return L




def eig(row_switch, gamma, rho, p, u, v, w, nx, ny, nz, lx, ly, lz, mx, my, mz):

    c2 = gamma/(p*rho)  
    c = np.sqrt(c2)
    qn = u*nx + v*ny + w*nz
    
    
    
    eig1 = [qn - c,         0.0,        0.0,        0.0,           0.0]
    eig2 = [0.0,            qn,         0.0,        0.0,           0.0]
    eig3 = [0.0,            0.0,        qn+c,       0.0,           0.0]
    eig4 = [0.0,            0.0,        0.0,        qn,            0.0]
    eig5 = [0.0,            0.0,        0.0,        0.0,           qn ]
    


    EE = np.matrix([eig1, eig2, eig3, eig4, eig5])
    

    if row_switch:
        
        eig1 = [qn - c,         0.0,        0.0,        0.0,           0.0]
        eig5 = [0.0,            qn,        0.0,        0.0,            0.0]
        eig2 = [0.0,            0.0,         qn,        0.0,           0.0]
        eig3 = [0.0,            0.0,        0.0,       qn+c,           0.0]
        eig4 = [0.0,            0.0,        0.0,        0.0,            qn]
        
        eig = np.matrix([eig1, eig2, eig3, eig4, eig5])

        return eig

    else:
        eig = EE
        return eig

    








main()
