import numpy as np
import pylab as pl



####
Ly = 1.
y_center = 0.5*Ly
rho_1 = 1.
rho_2 = 1.
Re_s = 640.
du = 1.
mu = 3E-5



rho_ref = 0.5*(rho_1 + rho_2)

nu = mu/rho_ref



sigma = Re_s * (nu/du)


temp = np.linspace(0, Ly, 100)

y = temp - y_center



xi = y / sigma

u = 0.5 * np.tanh(xi/2.)

pl.plot(u,y)
pl.show()


s = rho_1/rho_2

lam = (s-1)*(s+1)

rho = 1 + lam*np.tanh(xi/2.)

pl.plot(rho,y)
pl.show()


end_t = 820 * (sigma/du)

print end_t

