
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt 
import pandas as pd 

band1  = pd.read_csv("band1-wn1.csv",comment="#", sep='\\s+',  names=["bytes", "MBps"] )
band2  = pd.read_csv("band2-wn1.csv",comment="#", sep='\\s+',  names=["bytes", "MBps"] )
#band3  = pd.read_csv("band1-wn20.csv",comment="#", sep='\\s+',  names=["bytes", "MBps"] )
#band4  = pd.read_csv("band2-wn20.csv",comment="#", sep='\\s+',  names=["bytes", "MBps"] )

##bandOPA = pd.read_csv("band_opa.csv",comment="#", sep='\\s+',  names=["bytes", "MBps"] )
# bandSHM = pd.read_csv("band_shm.csv",comment="#", sep='\\s+',  names=["bytes", "MBps"] )

#print (bandPC.bytes)
#print (bandPC.MBps)

#sistemare nome e data
plt.title('Performance - "Network bandwidth" - Nna Minkousse Kenneth James 03/04/2026')
plt.grid()
plt.xlabel('bytes')
#plt.xscale('log')
plt.yscale('log')
plt.ylabel('MBps')

plt.plot(band1.bytes,band1.MBps,'ro-',label='wn1-wn1')
plt.plot(band2.bytes,band2.MBps,'go-',label='wn1-wn2')
#plt.plot(band3.bytes,band3.MBps,'bo-',label='wn20-wn20')
#plt.plot(band4.bytes,band4.MBps,'yo-',label='wn20-wn21')

plt.legend(loc='upper left')

plt.savefig('band.png')


