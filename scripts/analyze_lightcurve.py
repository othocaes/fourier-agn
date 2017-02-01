# -*- coding: utf-8 -*-
from __future__ import unicode_literals
import numpy as np
import sys
import getopt
import clag

sys.path.insert(1, "/usr/local/science/clag/")

# For jupyter notebook
# %pylab inline

try:
    opts, args = getopt.getopt(sys.argv[1:], "")
except getopt.GetoptError:
    print 'analyze_lightcure.py <reference curve> <compared curve>'
    sys.exit(2)

# Time resolution determined from inspection and testing. This script
# does not expect evenly spaced data in time.
dt = 0.1

### Get the psd for the #fqL = np.hstack((np.array(0.5*f1),np.logspace(np.log10(0.9*f1),
first light curve ###


# These bin values determined summer 2016 for STORM III optical/UV lightcurves
fqL = np.array([0.0049999999, 0.018619375, 0.044733049, 0.069336227,
                0.10747115, 0.16658029, 0.25819945, 0.40020915,
                0.62032418])

#A general rules for fqL is as follows:
#
#    define f1, f2 as the two extreme frequencies allowed by the data. i.e.
#    f1=1/T with T being the length of observation in time units, and
#    f2=0.5/Δt
#
#	The frequency limits where the psd/lag can be constrained are about
#	~0.9f1−0.5f2. The 0.9 factor doesn't depend on the data much, but
#	values in the range ~[0.9-1.1] are ok. The factor in front of f2
#	depends on the quality of the data, for low qualily data, use ~0.1--
#	0.2, and for high quality data, increase it up to 0.9−−1.
#
#	Always include two dummy bins at the low and high frequencies and
#	ignore them. The first and last bins are generally biased, So I suggest
#	using the first bin as 0.5f1−0.9f1 (or whatever value you use instead
#	of 0.9f1, see second point above), and the last bin should be
#	0.5f2−2∗f2 (or whatever value instead of 0.5f2, see second point
#	above). So the frequency bins should be something like:
#	[0.5f1,0.9f1,...,0.5f2,2f2], the bins in between can be devided as
#	desired.
#
#fqd is the bin center
#
# If lightcurves need to be split:
# seg_length = 256;
# fqL = np.logspace(np.log10(1.1/seg_length),np.log10(.5/dt),7)
# fqL = np.concatenate(([0.5/seg_length], fqL))
#

#f1 = 1/175.
#f2 = 0.5/dt
#fqL = np.hstack((np.array(0.5*f1),np.logspace(np.log10(0.9*f1),
#                np.log10(0.3*f2),9),np.array(2*f2)))
fqL = np.logspace(np.log10(0.0049999999),np.log10(0.62032418),9)
nfq = len(fqL) - 1
fqd = 10**(np.log10( (fqL[:-1]*fqL[1:]) )/2.)


## load the first light curve
lc1_time, lc1_strength, lc1_strength_err = 
    np.loadtxt(args[0],skiprows=1).T

# for pylab: errorbar(t1,l1,yerr=l1e,fmt='o')

# Used throughout
initial_args = np.ones(nfq)


## initialize the psd class for multiple light curves ##
P1  = clag.clag('psd10r',
                [lc1_time], [lc1_strength], [lc1_strength_err],
                dt, fqL)
ref_psd, ref_psd_err = clag.optimize(P1, initial_args)
ref_psd, ref_psd_err = clag.errors(P1, ref_psd, ref_psd_err)

## plot ##
#xscale('log'); ylim(-4,2)
#errorbar(fqd, ref_psd, yerr=ref_psd_err, fmt='o', ms=10)

# Load second light curve
lc2_time, lc2_strength, lc2_strength_err = np.loadtxt(args[1],skiprows=1).T
P2  = clag.clag('psd10r', [lc2_time], [lc2_strength], [lc2_strength_err], dt, fqL)
echo_psd, echo_psd_err = clag.optimize(P2, initial_args)
echo_psd, echo_psd_err = clag.errors(P2, echo_psd, echo_psd_err)



### Now the cross spectrum ###
### We also give it the calculated psd values as input ###
Cx = clag.clag('cxd10r',
				[[lc1_time,lc1_time]], 
            	[[lc1_strength,lc2_strength]],
            	[[lc1_strength_err,lc2_strength_err]], 
               dt, fqL, ref_psd, echo_psd)

#Cx_vals = np.concatenate( (0.3*(ref_psd*echo_psd)**0.5, ref_psd*0+1.) )
Cx_vals = np.concatenate( ((ref_psd+echo_psd)*0.5-0.3,ref_psd*0+0.1) )
Cx_vals, Cx_err = clag.optimize(Cx, Cx_vals)

#?????? %autoreload
Cx_vals, Cx_err = clag.errors(Cx,Cx_vals,Cx_err)

phi, phie = Cx_vals[nfq:], Cx_err[nfq:]
lag, lage = phi/(2*np.pi*fqd), phie/(2*np.pi*fqd)
cross_spectrm, cross_spectrm_err = Cx_vals[:nfq], Cx_err[:nfq]

np.savetxt("freq.out",fqL.reshape((-1,len(fqL))))
np.savetxt("ref_psd.out",[ref_psd,ref_psd_err])
np.savetxt("echo_psd.out",[echo_psd,echo_psd_err])
np.savetxt("crsspctrm.out",[cross_spectrm,cross_spectrm_err])
np.savetxt("timelag.out",[lag,lage])


