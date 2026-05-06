#!/usr/bin/env python2

import matplotlib
matplotlib.use('Agg')   # Backend per PNG 
import matplotlib.pyplot as plt
import pandas as pd

df1n = pd.read_csv("mpi_heat_original-1n.csv" , comment="#", names=["par", "nt", "r", "c", "iter", "t"])
df2n = pd.read_csv("mpi_heat_original-2n.csv" , comment="#", names=["par", "nt", "r", "c", "iter", "t"])


df1n1k = df1n[df1n["r"] == 1024]
df1n2k = df1n[df1n["r"] == 2048]

df2n1k = df2n[df2n["r"] == 1024]
df2n2k = df2n[df2n["r"] == 2048]

plt.subplot(2,1,1)

plt.title('MPI "HEAT scaling  1vs2 nodes(Overlap comunicazione)" - Nna Minkousse Kenneth James 22/04/26')

plt.grid()
plt.xlabel('tasks')
plt.ylabel('time')
plt.yscale('log')
plt.plot(df1n2k.nt,  df1n2k.t, '-o', label="2048-1node")
plt.plot(df2n2k.nt,  df2n2k.t, '-o', label="2048-2nodes")
plt.legend(shadow=True,loc="best")

plt.subplot(2,1,2)

plt.grid()
plt.xlabel('tasks')
plt.ylabel('speedup')
plt.plot(df1n2k.nt,  df1n2k.t.iloc[0]/df1n2k.t,  '-o', label="2048-1node")
plt.plot(df2n2k.nt,  df1n2k.t.iloc[0]/df2n2k.t,  '-o', label="2048-2nodes")
plt.plot(range(1,5),range(1,5),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('mpi_heat_original.png')
