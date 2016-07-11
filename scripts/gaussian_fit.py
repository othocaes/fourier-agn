import numpy as np
from astropy.modeling import models, fitting

# Using Models

# The astropy.modeling package defines a number of models that are collected under a single namespace as astropy.modeling.models. Models behave like parametrized functions:

from astropy.modeling import models
g = models.Gaussian1D(amplitude=1.2, mean=0.9, stddev=0.5)
print(g)
# Model: Gaussian1D
# Inputs: ('x',)
# Outputs: ('y',)
# Model set size: 1
# Parameters:
#     amplitude mean stddev
#     --------- ---- ------
#           1.2  0.9    0.5
#
# Model parameters can be accessed as attributes:

g.amplitude
# Parameter('amplitude', value=1.2)
g.mean
# Parameter('mean', value=0.9)
g.stddev
# Parameter('stddev', value=0.5)

# and can also be updated via those attributes:

g.amplitude = 0.8
g.amplitude
# Parameter('amplitude', value=0.8)

# Models can be evaluated by calling them as functions:

g(0.1)
# 0.22242984036255528
g(np.linspace(0.5, 1.5, 7))
# array([ 0.58091923,  0.71746405,  0.7929204 ,  0.78415894,  0.69394278,
#         0.54952605,  0.3894018 ])



import numpy as np
import matplotlib.pyplot as plt
from astropy.modeling import models, fitting

# Generate fake data
np.random.seed(0)
x = np.linspace(-5., 5., 200)
y = 3 * np.exp(-0.5 * (x - 1.3)**2 / 0.8**2)
y += np.random.normal(0., 0.2, x.shape)

# Fit the data using a box model
t_init = models.Trapezoid1D(amplitude=1., x_0=0., width=1., slope=0.5)
fit_t = fitting.LevMarLSQFitter()
t = fit_t(t_init, x, y)

# Fit the data using a Gaussian
g_init = models.Gaussian1D(amplitude=1., mean=0, stddev=1.)
fit_g = fitting.LevMarLSQFitter()
g = fit_g(g_init, x, y)

# Plot the data with the best-fit model
plt.figure(figsize=(8,5))
plt.plot(x, y, 'ko')
plt.plot(x, t(x), label='Trapezoid')
plt.plot(x, g(x), label='Gaussian')
plt.xlabel('Position')
plt.ylabel('Flux')
plt.legend(loc=2)

plt.show()