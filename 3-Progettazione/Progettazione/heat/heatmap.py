import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from numpy import genfromtxt

a = genfromtxt('heatmap.txt', delimiter=' ')
plt.imsave('heatmap.png',a, cmap='hot')
