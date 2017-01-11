# -*- coding: utf-8 -*-
from __future__ import unicode_literals
import numpy as np
import sys
import getopt

sys.path.insert(1,"/usr/local/science/clag/")
import clag

# psd1_output = open("psd1.tmp")
# psd2_output = open("psd2.tmp")
# lag_output = open("lag.tmp")

# For jupyter notebook
# %pylab inline

try:
    opts,args = getopt.getopt(sys.argv[1:], "")
except getopt.GetoptError:
    print 'analyze_lightcure.py <reference curve> <compared curve>'
    sys.exit(2)

## load the first light curve
lc1_table = np.loadtxt(args[0],skiprows=1)

# works if first two entries represent minimum spacing, from example
# dt = lc1_table[1,0] - lc1_table[0, 0]

# Time resolution determined from inspection and testing. This script
# does not expect evenly spaced data in time.
dt = 0.1


# _ = plot(lc1_table[:,0], lc1_table[:,1])
# _ = plot(lc1_table[:,0], lc1_table[:,3])


# Split the light curve into segments #
#seg_length = 256
#index = np.arange(len(data)).reshape((-1, seg_length))

# For now, instead of splitting up the curves, the program will assume 
# that the data list is shorter than 256 elemements. so,

index = np.arange(len(lc1_table)).reshape(-1,len(lc1_table))

lc1_time = [lc1_table[i, 0] for i in index]
lc1_strength  = [lc1_table[i, 1] for i in index]
lc1_strength_err = [lc1_table[i, 2] for i in index]

#### Get the psd for the first light curve ####

# These bin values determined summer 2016 for STORM III optical/UV lightcurves
fqL = np.array([0.0049999999, 0.018619375, 0.044733049, 0.069336227, 0.10747115, 0.16658029, 0.25819945, 0.40020915, 0.62032418])

# using utilities to set up frequency bins #
# fqL = np.logspace(np.log10(1.1/seg_length),np.log10(.5/dt),7)
# fqL = np.concatenate(([0.5/seg_length], fqL))

nfq = len(fqL) - 1


## initialize the psd class for multiple light curves ##
P1  = clag.clag('psd', lc1_time, lc1_strength, lc1_strength_err, dt, fqL)





## initial parameters, start with ones
inpars = np.ones(nfq)

## print the loglikelihood for the input values ##
P1.logLikelihood(inpars)



## Now do the fitting and find the best fit psd values at the given frequency bins ##
psd1, psd1e = clag.optimize(P1, inpars)




## plot ##
fqd = 10**(np.log10( (fqL[:-1]*fqL[1:]) )/2.)

#loglog(fqd, 0.1*fqd**(-1.5), label='input psd')
#errorbar(fqd[1:-1], psd1[1:-1], yerr=psd1e[1:-1], fmt='o', ms=10, label='fit')

# load second lightcurve

# This would work if both curves are in same file
# lc2_strength  = [lc1_table[i, 3] for i in index]
# lc2_strength_err = [lc1_table[i, 4] for i in index]

# But, they aren't, so,

# Load second light curve
lc2_table = np.loadtxt(args[1],skiprows=1)

index = np.arange(len(lc2_table)).reshape(-1,len(lc2_table))

lc2_time = [lc2_table[i, 0] for i in index]
lc2_strength  = [lc2_table[i, 1] for i in index]
lc2_strength_err = [lc2_table[i, 2] for i in index]



## Now do the second light curve

P2  = clag.clag('psd', lc2_time, lc2_strength, lc2_strength_err, dt, fqL)

psd2, psd2e = clag.optimize(P2, inpars)






### Now the cross spectrum ###
### We also give it the calculated psd values as input ###
Cx = clag.clag('cxd', 
               [list(i) for i in zip(lc1_time,lc1_time)], 
               [list(i) for i in zip(lc1_strength,lc2_strength)],
               [list(i) for i in zip(lc1_strength_err,lc2_strength_err)], 
               dt, fqL, psd1, psd2)

inpars = np.concatenate( (0.3*(psd1*psd2)**0.5, psd1*0+1.) )
p, pe = clag.optimize(Cx, inpars)
phi, phie = p[nfq:], pe[nfq:]
lag, lage = phi/(2*np.pi*fqd), phie/(2*np.pi*fqd)
cx, cxe   = p[:nfq], pe[:nfq]

np.savetxt("lag.out",lag)








## plot ##

#semilogx(fqd, fqd*0+1.0, label='input phase lag')
#ylim([0.8, 1.2])
#errorbar(fqd[1:-1], phi[1:-1], yerr=phie[1:-1], fmt='o', ms=10, label='fit')

