
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

lat1 = pd.read_csv("lat1-wn1.csv",comment="#", sep='\\s+',  names=["avgTround", "avgT"] )
lat2 = pd.read_csv("lat2-wn1.csv",comment="#", sep='\\s+',  names=["avgTround", "avgT"] )
#lat3 = pd.read_csv("lat1-wn20.csv",comment="#", sep='\\s+',  names=["avgTround", "avgT"] )
#lat4 = pd.read_csv("lat2-wn20.csv",comment="#", sep='\\s+',  names=["avgTround", "avgT"] )

#print (lat1.bytes)
#print (lat1.MBps)
# sistemare nome e data
plt.title('Performance - "Network latency" - Nna Minkousse Kenneth James 04/03/2026')
plt.grid()
plt.ylabel('microsec')
plt.yscale('log')

plt.bar("1 node", lat1.avgT,    label='wn1-wn1')
plt.bar("2 nodes", lat2.avgT,    label='wn1-wn2')
#plt.bar("1 node HPC", lat3.avgT,    label='wn20-wn20')
#plt.bar("2 nodes HPC", lat4.avgT,    label='wn20-wn21')




plt.legend(loc='upper left')

plt.savefig('lat.png')


