# -*- coding: utf-8 -*-
from __future__ import unicode_literals
import numpy as np
import clag
import sys


# For jupyter notebook
# %pylab inline

try:
    opts,args = getopt.getopt(args, "")
except getopt.GetoptError:
    print 'analyze_lightcure.py <reference curve> <compared curve>'
    sys.exit(2)

## load the first light curve
lc1 = np.loadtxt(args[0])

# works if first two entries represent minimum spacing, from example
# dt = lc1[1,0] - lc1[0, 0]

# Time resolution determined from inspection and testing.
dt = 0.01


_ = plot(lc1[:,0], lc1[:,1])
_ = plot(lc1[:,0], lc1[:,3])

# Split the light curve into segments #
seg_length = 256
index = np.arange(len(lc1)).reshape((-1, seg_length))

lc1_time = [lc1[i, 0] for i in index]
lc1_strength  = [lc1[i, 1] for i in index]
lc1_strength_err = [lc1[i, 2] for i in index]

# This would work if both curves are in same file
# Lc2  = [lc1[i, 3] for i in index]
#Lc2e = [lc1[i, 4] for i in index]



#### Get the psd for the first light curve ####

# These bin values determined summer 2015 for STORM III optical/UV lightcurves
fqL = [0.0049999999, 0.018619375, 0.044733049, 0.069336227, 0.10747115, 0.16658029, 0.25819945, 0.40020915, 0.62032418]

# using utilities to set up frequency bins #
# fqL = np.logspace(np.log10(1.1/seg_length),np.log10(.5/dt),7)
# fqL = np.concatenate(([0.5/seg_length], fqL))

nfq = len(fqL) - 1



## initialize the psd class for multiple light curves ##
P1  = clag.clag('psd', lc1_time, lc1_strength, lc1_strength_err, dt, fqL)





## initial parameters, start with ones
inpars = np.ones(nfq)

## print the loglikelihood for the input values ##
print P1.logLikelihood(inpars)



## Now do the fitting and find the best fit psd values at the given frequency bins ##
psd1, psd1e = clag.optimize(P1, inpars)




## plot ##
fqd = 10**(np.log10( (fqL[:-1]*fqL[1:]) )/2.)

loglog(fqd, 0.1*fqd**(-1.5), label='input psd')
errorbar(fqd[1:-1], psd1[1:-1], yerr=psd1e[1:-1], fmt='o', ms=10, label='fit')




## Now do the second light curve

P2  = clag.clag('psd', lc1_time, Lc2, Lc2e, dt, fqL)

psd2, psd2e = clag.optimize(P2, inpars)






### Now the cross spectrum ###
### We also give it the calculated psd values as input ###
Cx = clag.clag('cxd', 
               [list(i) for i in zip(lc1_time,lc1_time)], 
               [list(i) for i in zip(lc1_strength,Lc2)],
               [list(i) for i in zip(lc1_strength_err,Lc2e)], 
               dt, fqL, psd1, psd2)

inpars = np.concatenate( (0.3*(psd1*psd2)**0.5, psd1*0+1.) )
p, pe = clag.optimize(Cx, inpars)
phi, phie = p[nfq:], pe[nfq:]
lag, lage = phi/(2*np.pi*fqd), phie/(2*np.pi*fqd)
cx, cxe   = p[:nfq], pe[:nfq]








## plot ##

semilogx(fqd, fqd*0+1.0, label='input phase lag')
ylim([0.8, 1.2])
errorbar(fqd[1:-1], phi[1:-1], yerr=phie[1:-1], fmt='o', ms=10, label='fit')

